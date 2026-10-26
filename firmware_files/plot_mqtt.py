import collections

import matplotlib.pyplot as plt
import paho.mqtt.client as mqtt
from matplotlib.animation import FuncAnimation

BROKER = "broker.emqx.io"
PORT = 1883
TOPIC = "emqx/esp32"

# ~10s of history at 20Hz
WINDOW = 200

t = collections.deque(maxlen=WINDOW)
roll = collections.deque(maxlen=WINDOW)
pitch = collections.deque(maxlen=WINDOW)
stats = {"reps": 0, "sets": 0}


def on_message(client, userdata, msg):
    # Public broker, so ignore anything that isn't ours
    try:
        ms, r, p, reps, sets = msg.payload.decode().split(",")
        t.append(int(ms) / 1000)
        roll.append(float(r))
        pitch.append(float(p))
        stats["reps"], stats["sets"] = int(reps), int(sets)
    except ValueError:
        pass


client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)
client.username_pw_set("emqx", "public")
client.on_connect = lambda c, *_: c.subscribe(TOPIC)
client.on_message = on_message
client.connect(BROKER, PORT)
client.loop_start()

fig, ax = plt.subplots()
roll_line, = ax.plot([], [], label="roll")
pitch_line, = ax.plot([], [], label="pitch")
ax.set_xlabel("time (s)")
ax.set_ylabel("angle (deg)")
ax.set_ylim(-180, 180)
ax.legend(loc="upper left")


def update(_):
    if t:
        roll_line.set_data(t, roll)
        pitch_line.set_data(t, pitch)
        ax.set_xlim(t[0], max(t[-1], t[0] + 1))
    ax.set_title(f"reps: {stats['reps']}   sets: {stats['sets']}")
    return roll_line, pitch_line


anim = FuncAnimation(fig, update, interval=50, cache_frame_data=False)
plt.show()
client.loop_stop()
