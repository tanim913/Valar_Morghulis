"""Generates the sound effects in assets/audio/ from scratch (no samples, so no licences).

    python3 tools/make_sfx.py        needs numpy
"""
import os
import wave
import numpy as np

SR = 22050
OUT = os.path.join(os.path.dirname(__file__), "..", "assets", "audio")
rng = np.random.default_rng(913)


def t(seconds):
    return np.arange(int(SR * seconds)) / SR


def decay(x, rate):
    return np.exp(-rate * x)


def tone(freq, seconds, rate=8.0, harmonics=((1, 1.0), (2, 0.25), (3, 0.1))):
    x = t(seconds)
    f = np.broadcast_to(freq, x.shape) if np.ndim(freq) == 0 else freq
    phase = 2 * np.pi * np.cumsum(f) / SR
    return sum(a * np.sin(h * phase) for h, a in harmonics) * decay(x, rate)


def lowpass(x, cutoff):
    a = np.clip(cutoff, 0.001, 0.999)
    a = np.broadcast_to(a, x.shape)
    y = np.zeros_like(x)
    acc = 0.0
    for i, v in enumerate(x):
        acc += a[i] * (v - acc)
        y[i] = acc
    return y


def place(parts, total):
    out = np.zeros(int(SR * total))
    for start, sig in parts:
        i = int(SR * start)
        out[i:i + len(sig)] += sig[: len(out) - i]
    return out


def save(name, x, gain=0.8):
    x = x / max(1e-9, np.max(np.abs(x))) * gain
    x[:40] *= np.linspace(0, 1, 40)
    x[-200:] *= np.linspace(1, 0, 200)
    with wave.open(os.path.join(OUT, name), "wb") as w:
        w.setnchannels(1)
        w.setsampwidth(2)
        w.setframerate(SR)
        w.writeframes((x * 32767).astype(np.int16).tobytes())


# a bright two-note chime: an egg is caught
save("catch.wav", place([(0, tone(988, 0.30, 9)), (0.07, tone(1480, 0.38, 8))], 0.45))

# a thud and a rumble: the dragon is hurt
x = t(0.40)
thud = np.sin(2 * np.pi * np.cumsum(150 * decay(x, 5) + 45) / SR) * decay(x, 7)
rumble = lowpass(rng.standard_normal(len(x)), 0.06 + 0.2 * decay(x, 9)) * decay(x, 6) * 2.5
save("hurt.wav", thud + rumble)

# a rushing breath: the dragon fires
x = t(0.28)
whoosh = lowpass(rng.standard_normal(len(x)), 0.55 * decay(x, 7) + 0.04) * np.sin(np.pi * np.minimum(1, x / 0.28)) ** 0.6
save("fire.wav", whoosh, 0.7)

# a short crack: a shot lands
x = t(0.18)
crack = np.sin(2 * np.pi * np.cumsum(520 * decay(x, 14) + 110) / SR) * decay(x, 16)
click = rng.standard_normal(len(x)) * decay(x, 60) * 0.6
save("hit.wav", crack + click)

# a rising fanfare: a level is won
notes = [523, 659, 784, 1047]
save("win.wav", place([(0.11 * i, tone(f, 0.55, 5)) for i, f in enumerate(notes)] + [(0.46, tone(1319, 0.7, 4))], 1.2))

# a falling line: the game is over
notes = [440, 392, 349, 262]
save("lose.wav", place([(0.22 * i, tone(f, 0.55, 4, ((1, 1), (2, 0.4), (3, 0.3), (4, 0.15)))) for i, f in enumerate(notes)], 1.4))
