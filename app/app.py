import serial
import time
import socket
import struct

ser = serial.Serial("/dev/ttyACM0", 2000000, timeout=10)
time.sleep(2)

# UDP server settings
UDP_IP = "0.0.0.0"
UDP_PORT = 5000

# Struct format for the base telemetry data 
TELEMETRY_STRUCT_FORMAT = (
    "<"      # Little-endian
    "i"      # IsRaceOn
    "I"      # TimestampMS
    "fff"    # EngineMaxRpm, EngineIdleRpm, CurrentEngineRpm
    "fff"    # AccelerationX, Y, Z
    "fff"    # VelocityX, Y, Z
    "fff"    # AngularVelocityX, Y, Z
    "fff"    # Yaw, Pitch, Roll
    "ffff"   # NormalizedSuspensionTravel FL, FR, RL, RR
    "ffff"   # TireSlipRatio FL, FR, RL, RR
    "ffff"   # WheelRotationSpeed FL, FR, RL, RR
    "iiii"   # WheelOnRumbleStrip FL, FR, RL, RR
    "ffff"   # WheelInPuddleDepth FL, FR, RL, RR
    "ffff"   # SurfaceRumble FL, FR, RL, RR
    "ffff"   # TireSlipAngle FL, FR, RL, RR
    "ffff"   # TireCombinedSlip FL, FR, RL, RR
    "ffff"   # SuspensionTravelMeters FL, FR, RL, RR
    "iiiii"  # CarOrdinal, CarClass, CarPerformanceIndex, DrivetrainType, NumCylinders
)

# Extended format (for FH4 and FM7 Dash Mode)
EXTENDED_FORMAT = (
    "fff"     # PositionX, Y, Z
    "fff"     # Speed, Power, Torque
    "ffff"    # TireTemp FL, FR, RL, RR
    "fff"     # Boost, Fuel, DistanceTraveled
    "fff"     # BestLap, LastLap, CurrentLap
    "f"       # CurrentRaceTime
    "H"       # LapNumber
    "B"       # RacePosition
    "BBBBB"   # Accel, Brake, Clutch, HandBrake, Gear
    "b"       # Steer
    "bb"      # NormalizedDrivingLine, NormalizedAIBrakeDifference
)

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((UDP_IP, UDP_PORT))
print(f"Listening for Forza telemetry on {UDP_IP}:{UDP_PORT}...")

while True:
    data, addr = sock.recvfrom(512)
    #print(f"Received {len(data)} bytes from {addr}")

    game_title = "Unknown"
    if len(data) == 232:
        game_title = "Forza Motorsport 7 Sled"
    elif len(data) == 311:
        game_title = "Forza Motorsport 7 Dash"
    elif len(data) == 324:
        game_title = "Forza Horizon 4"
        # Normalize 
        normalized = data[:232] + data[244:244 + 79]
        data = normalized
    else:
        print("Unknown packet size, skipping")
        continue

    base_size = struct.calcsize(TELEMETRY_STRUCT_FORMAT)
    base_data = struct.unpack(TELEMETRY_STRUCT_FORMAT, data[:base_size])

    # print(f"Game: {game_title}")
    # print("Base Telemetry:")
    speed_offset = 244 
    speed = int(struct.unpack_from('<f', data, speed_offset)[0] * 3.6)
    print(speed)
    res = bytes([int(base_data[4]/100)])
    speed1 = bytes([int(speed/256)])
    speed2 = bytes([int(speed%256)])
    # print(speed1)
    # ser.write(res)
    ser.write(speed1)
    ser.write(speed2)
    print(" ".join(f"{byte:02x}" for byte in speed1), end=" / ")
    print(" ".join(f"{byte:02x}" for byte in speed2))


    # if game_title in ["Forza Horizon 4", "Forza Motorsport 7 Dash"]:
    #     extended_format = "<" + EXTENDED_FORMAT
    #     extended_data = struct.unpack_from(extended_format, data, offset=base_size)
        
    #     print("Extended Telemetry:")
    #     print(extended_data)




def send(i):
    res = bytes([i])
    print(" ".join(f"{byte:02x}" for byte in res))

    ser.write(res)
    data_bytes = ser.read()
    print(' '.join(f'{byte:02x}' for byte in data_bytes))
    print()


# for i in range(1, 50):
#     send(0)
#     time.sleep(1)
#     send(100)
#     time.sleep(1)


# ser.close()
