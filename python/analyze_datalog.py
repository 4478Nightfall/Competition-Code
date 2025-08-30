import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# Load the data
filename = 'D:\datalog.csv'  # Change path if needed
try:
    df = pd.read_csv(filename)
except FileNotFoundError:
    print(f"File '{filename}' not found. Make sure it's in the same directory as this script.")
    exit(1)

print("Columns:", list(df.columns))
print("\nFirst All:") 
print(df.head())

# Birdseye view path plot using odometry
WHEEL_DIAMETER = 2.75  # inches
TRACK_WIDTH = 13.0    # inches
H_TRACK_DIAMETER = 2.75  # inches

def rot_to_dist(rot, diameter):
    return np.pi * diameter * rot

left_start = df['left_deg'].iloc[0]
left_end = df['left_deg'].iloc[-1]
right_start = df['right_deg'].iloc[0]
right_end = df['right_deg'].iloc[-1]

left_rot = left_end - left_start
right_rot = right_end - right_start
avg_rot = (left_rot + right_rot) / 2

print("Left encoder start:", left_start, "end:", left_end)
print("Right encoder start:", right_start, "end:", right_end)
print("Total left rotations:", left_rot)
print("Total right rotations:", right_rot)
print("Average rotations:", avg_rot)
print("WHEEL_DIAMETER:", WHEEL_DIAMETER)
print("Estimated distance traveled (inches):", np.pi * WHEEL_DIAMETER * avg_rot)

# 2D Odometry (for reference, using only left/right wheels)
x, y, theta = 0.0, 0.0, 0.0
positions_2d = [(x, y)]

left_prev = df['left_deg'].iloc[0]
right_prev = df['right_deg'].iloc[0]

for i in range(1, len(df)):
    left_curr = df['left_deg'].iloc[i]
    right_curr = df['right_deg'].iloc[i]
    d_left = rot_to_dist(left_curr - left_prev, WHEEL_DIAMETER)
    d_right = rot_to_dist(right_curr - right_prev, WHEEL_DIAMETER)
    left_prev, right_prev = left_curr, right_curr

    d_center = (d_left + d_right) / 2.0
    d_theta = (d_right - d_left) / TRACK_WIDTH

    theta += d_theta
    x += d_center * np.cos(theta)
    y += d_center * np.sin(theta)
    positions_2d.append((x, y))

positions_2d = np.array(positions_2d)

print("X range:", positions_2d[:,0].min(), "to", positions_2d[:,0].max())
print("Y range:", positions_2d[:,1].min(), "to", positions_2d[:,1].max())

plt.figure(figsize=(8,8))
plt.plot(positions_2d[:,0], positions_2d[:,1], label='Robot Path (2D)')
plt.xlabel('X Position (inches)')
plt.ylabel('Y Position (inches)')
plt.title('Robot Route (Birdseye View, 2D)')
plt.axis('equal')
plt.legend()
plt.show(block=False)



# Plot motor temperatures over time
plt.figure(figsize=(10,6))
plt.plot(df['timestamp'], df['left_temp'], label='Left Motors Temp')
plt.plot(df['timestamp'], df['right_temp'], label='Right Motors Temp')
plt.plot(df['timestamp'], df['intake_temp'], label='Intake Temp')
plt.xlabel('Time (ms)')
plt.ylabel('Temperature (°C)')
plt.title('Motor Temperatures Over Time')
plt.legend()
plt.tight_layout()
plt.show(block=False)

# Plot motor velocities over time
plt.figure(figsize=(10,6))
plt.plot(df['timestamp'], df['left_vel'], label='Left Motors Vel')
plt.plot(df['timestamp'], df['right_vel'], label='Right Motors Vel')
plt.plot(df['timestamp'], df['intake_vel'], label='Intake Vel')
plt.xlabel('Time (ms)')
plt.ylabel('Velocity (RPM)')
plt.title('Motor Velocities Over Time')
plt.legend()
plt.tight_layout()
plt.show(block=False)

# Plot motor positions over time
plt.figure(figsize=(10,6))
plt.plot(df['timestamp'], df['left_deg'], label='Left Motors Deg')
plt.plot(df['timestamp'], df['right_deg'], label='Right Motors Deg')
plt.plot(df['timestamp'], df['intake_deg'], label='Intake Deg')
plt.xlabel('Time (ms)')
plt.ylabel('Position (deg)')
plt.title('Motor Positions Over Time')
plt.legend()
plt.tight_layout()
plt.show()

# Print summary statistics
print("\nSummary statistics:")
print(df.describe())

print("np.pi:", np.pi)
print("rot:", df['left_deg'].iloc[-1] - df['left_deg'].iloc[0])
print("Computed distance:", np.pi * WHEEL_DIAMETER * (df['left_deg'].iloc[-1] - df['left_deg'].iloc[0])) 