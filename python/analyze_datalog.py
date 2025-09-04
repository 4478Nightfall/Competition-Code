import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# Load the data
fileName = 'D:\datalog.csv'  # Change path if needed
try:
    df = pd.read_csv(fileName)
except FileNotFoundError:
    print(
        f"File '{fileName}' not found. Make sure it's in the same directory as this script.")
    exit(1)

print("Columns:", list(df.columns))
print("\nFirst All:")
print(df.head())

# Birdseye view path plot using odometry
wheelDiameter = 2.75  # inches
trackWidth = 13.0    # inches
horizontalTrackingWheelDiameter = 2.00  # inches


def rotToDist(rot, diameter):
    return np.pi * diameter * rot


leftStart = df['left_deg'].iloc[0]
leftEnd = df['left_deg'].iloc[-1]
rightStart = df['right_deg'].iloc[0]
rightEnd = df['right_deg'].iloc[-1]

leftRot = leftEnd - leftStart
rightRot = rightEnd - rightStart
avgRot = (leftRot + rightRot) / 2

print("Left encoder start:", leftStart, "end:", leftEnd)
print("Right encoder start:", rightStart, "end:", rightEnd)
print("Total left rotations:", leftRot)
print("Total right rotations:", rightRot)
print("Average rotations:", avgRot)
print("wheelDiameter:", wheelDiameter)
print("Estimated distance traveled (inches):",
      np.pi * wheelDiameter * avgRot)

# 2D Odometry (for reference, using only left/right wheels)
x, y, theta = 0.0, 0.0, 0.0
positions2d = [(x, y)]

leftPrev = df['left_deg'].iloc[0]
rightPrev = df['right_deg'].iloc[0]

for i in range(1, len(df)):
    leftCurr = df['left_deg'].iloc[i]
    rightCurr = df['right_deg'].iloc[i]
    dLeft = rotToDist(leftCurr - leftPrev, wheelDiameter)
    dRight = rotToDist(rightCurr - rightPrev, wheelDiameter)
    leftPrev, rightPrev = leftCurr, rightCurr

    dCenter = (dLeft + dRight) / 2.0
    dTheta = (dRight - dLeft) / trackWidth

    theta += dTheta
    x += dCenter * np.cos(theta)
    y += dCenter * np.sin(theta)
    positions2d.append((x, y))

positions2d = np.array(positions2d)

print("X range:", positions2d[:, 0].min(), "to", positions2d[:, 0].max())
print("Y range:", positions2d[:, 1].min(), "to", positions2d[:, 1].max())

plt.figure(figsize=(8, 8))
plt.plot(positions2d[:, 0], positions2d[:, 1], label='Robot Path (2D)')
plt.xlabel('X Position (inches)')
plt.ylabel('Y Position (inches)')
plt.title('Robot Route (Birdseye View, 2D)')
plt.axis('equal')
plt.legend()
plt.show(block=False)


# Plot motor temperatures over time
plt.figure(figsize=(10, 6))
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
plt.figure(figsize=(10, 6))
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
plt.figure(figsize=(10, 6))
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
print("Computed distance:", np.pi * wheelDiameter *
      (df['left_deg'].iloc[-1] - df['left_deg'].iloc[0]))
