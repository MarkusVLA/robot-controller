import numpy as np
import matplotlib.pyplot as plt
from matplotlib.ticker import FuncFormatter

# Time domain data
time_file = "SDS2204X Plus_CSV_C1_1.csv"

times = []
voltages = []
with open(time_file, 'r') as file:
    header_passed = False
    for line in file:
        if 'Second' in line:
            header_passed = True
            continue
        if header_passed and ',' in line:
            parts = line.strip().split(',')
            if len(parts) >= 2:
                times.append(float(parts[0]))
                voltages.append(float(parts[1]) * 1000)  # Convert to mV

# FFT data
fft_file = "SDS2204X Plus_CSV_F1_1.csv"
frequencies = []
magnitudes = []
with open(fft_file, 'r') as file:
    header_passed = False
    for line in file:
        if 'Frequency,Real part' in line:
            header_passed = True
            continue
        if header_passed and ',' in line:
            parts = line.strip().split(',')
            if len(parts) >= 4:  # Frequency, Real, Imaginary, Magnitude(dB)
                frequencies.append(float(parts[0]))
                magnitudes.append(float(parts[3]))  # Magnitude in dB

def time_formatter(x, pos):
    if abs(x) >= 1e-3:
        return f"{x*1e3:.1f} ms"  # Convert to milliseconds
    else:
        return f"{x*1e6:.1f} µs"  # Convert to microseconds

def freq_formatter(x, pos):
    if x >= 1e6:
        return f"{x/1e6:.0f} MHz"
    elif x >= 1e3:
        return f"{x/1e3:.0f} kHz"
    else:
        return f"{x:.0f} Hz"

plt.figure(figsize=(12, 8))

plt.subplot(2, 1, 1)
plt.plot(times, voltages)
plt.xlabel('Time')
plt.ylabel('Voltage (mV)')
plt.title('Time Domain Signal - AC Coupled')
plt.grid(True)
plt.gca().xaxis.set_major_formatter(FuncFormatter(time_formatter))

plt.subplot(2, 1, 2)
plt.semilogx(frequencies, magnitudes)
plt.xlabel('Frequency')
plt.ylabel('Magnitude (dB)')
plt.title('FFT (Averaged over 128 samples) - AC Coupled')
plt.grid(True, which='both')
plt.gca().xaxis.set_major_formatter(FuncFormatter(freq_formatter))

plt.gca().grid(True, which='minor', linestyle=':', alpha=0.4)

plt.figtext(0.5, 0.01, 'Note: All measurements taken with AC coupling', 
           ha='center', fontsize=10, style='italic')

plt.tight_layout(rect=[0, 0.03, 1, 1])  
plt.show()
