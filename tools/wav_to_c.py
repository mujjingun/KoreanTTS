import wave, struct

def unpack(s):
	l = struct.unpack("<%sh" % (len(s) // 2), s)
	l = [s / 32767.0 for s in l]
	return l

src = wave.open("voice.wav", "rb")
frames = src.readframes(src.getnframes())
frames = unpack(frames)

frames = ['{0:.5f},'.format(s) for s in frames]

print(''.join(frames))
