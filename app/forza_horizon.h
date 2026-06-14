// Data format for Forza Horizon 4/5/6
// Theses macro defines the offset for easy usage.

//S -- Signed Integer
//U -- Unsigned Integer
//F -- Floating Point

#define FORZAHORIZON
#define FHBufferOffset 12


//S32 IsRaceOn; = 1 when race is on. = 0 when in menus/race stopped.
#define IsRaceOn 0

// U32 TimestampMS; Can overflow to 0 eventually
#define TimestampMS 4

// F32 Engine RPM values
#define EngineMaxRpm 8
#define EngineIdleRpm 12
#define CurrentEngineRpm 16

// F32 In the car's local space; X = right, Y = up, Z = forward
#define AccelerationX 20
#define AccelerationY 24
#define AccelerationZ 28

// F32 In the car's local space; X = right, Y = up, Z = forward
#define VelocityX 32
#define VelocityY 36
#define VelocityZ 40

// F32 Angular velocity in the car's local space (rad/s); X = pitch, Y = yaw, Z = roll
#define AngularVelocityX 44
#define AngularVelocityY 48
#define AngularVelocityZ 52 

// F32 Car orientation (radians)
#define Yaw 56
#define Pitch 60
#define Roll 64 

// F32 Suspension travel normalized: 0.0f = max stretch; 1.0 = max compression
#define NormalizedSuspensionTravelFrontLeft 68
#define NormalizedSuspensionTravelFrontRigh 72
#define NormalizedSuspensionTravelRearLeft 76
#define NormalizedSuspensionTravelRearRight 80 

// F32 Tire normalized slip ratio, = 0 means 100% grip and |ratio| > 1.0 means loss of grip.
#define TireSlipRatioFrontLeft 84
#define TireSlipRatioFrontRight 88
#define TireSlipRatioRearLeft 92
#define TireSlipRatioRearRight 96 

// F32 Wheel rotation speed radians/sec.
#define WheelRotationSpeedFrontLeft 100
#define WheelRotationSpeedFrontRight 104
#define WheelRotationSpeedRearLeft 108
#define WheelRotationSpeedRearRight 1012

// S32 = 1 when wheel is on rumble strip, = 0 when off.
#define WheelOnRumbleStripFrontLeft 116
#define WheelOnRumbleStripFrontRight 120
#define WheelOnRumbleStripRearLeft 124
#define WheelOnRumbleStripRearRight 128

// S32 = 1 when wheel is in a puddle, = 0 when not.
#define WheelInPuddleFrontLeft 132
#define WheelInPuddleFrontRight 136
#define WheelInPuddleRearLeft 140
#define WheelInPuddleRearRight 144

// F32 Non-dimensional surface rumble values passed to controller force feedback
#define SurfaceRumbleFrontLeft 148
#define SurfaceRumbleFrontRight 152
#define SurfaceRumbleRearLeft 156
#define SurfaceRumbleRearRight 160

// F32 Tire normalized slip angle, = 0 means 100% grip and |angle| > 1.0 means loss of grip.
#define TireSlipAngleFrontLeft 164
#define TireSlipAngleFrontRight 168
#define TireSlipAngleRearLeft 172
#define TireSlipAngleRearRight 176

// F32 Tire normalized combined slip, = 0 means 100% grip and |slip| > 1.0 means loss of grip.
#define TireCombinedSlipFrontLeft 180
#define TireCombinedSlipFrontRight 184
#define TireCombinedSlipRearLeft 188
#define TireCombinedSlipRearRight 192

// F32 Actual suspension travel in meters
#define SuspensionTravelMetersFrontLeft 196
#define SuspensionTravelMetersFrontRight 200
#define SuspensionTravelMetersRearLeft 204
#define SuspensionTravelMetersRearRight 208

// S32 Unique ID of the car make/model
#define CarOrdinal 212

// S32 Between 0 (D -- worst cars) and 7 (X class -- best cars) inclusive
#define CarClass 216

// S32 Between 100 (worst car) and 999 (best car) inclusive
#define CarPerformanceIndex 220

// S32 0 = FWD, 1 = RWD, 2 = AWD
#define DrivetrainType 224

// S32 Number of cylinders in the engine
#define NumCylinders 228

#ifdef FORZAHORIZON
// U32 Car group identifier
#define CarGroup 232

// F32 Velocity loss from smashable object collision (m/s)
#define SmashableVelDiff 236

// F32 Mass of recently hit smashable object (kg)
#define SmashableMass 240
#endif

// F32 Position in world space (meters)
#define PositionX 232 + FHBufferOffset
#define PositionY 236 + FHBufferOffset
#define PositionZ 240 + FHBufferOffset

// F32 Speed in meters per second
#define Speed 244 + FHBufferOffset

// F32 Power in watts
#define Power 248 + FHBufferOffset

// F32 Torque in newton-meters
#define Torque 252 + FHBufferOffset

// F32 Tire temperature
#define TireTempFrontLeft 256 + FHBufferOffset
#define TireTempFrontRight 260 + FHBufferOffset
#define TireTempRearLeft 264 + FHBufferOffset
#define TireTempRearRight 268 + FHBufferOffset

// F32 Turbo/supercharger boost (PSI above atmospheric)
#define Boost 272 + FHBufferOffset

// F32 Fuel level (0.0 = empty, 1.0 = full)
#define Fuel 276 + FHBufferOffset

// F32 Total distance traveled (meters)
#define DistanceTraveled 280 + FHBufferOffset

// F32 Lap times (seconds); 0.0 if not applicable
#define BestLapTime 284 + FHBufferOffset
#define LastLapTime 288 + FHBufferOffset
#define CurrentLapTime 292 + FHBufferOffset

// F32 Total race time (seconds since driving started)
#define CurrentRaceTime 296 + FHBufferOffset

// U16 Number of laps completed
#define LapNumber 300 + FHBufferOffset

// U8 Current race position
#define RacePosition 302 + FHBufferOffset

// U8 Player inputs (0 to 255)
#define Accelerator 303 + FHBufferOffset
#define Brake 304 + FHBufferOffset
#define Clutch 305 + FHBufferOffset
#define Handbrake 306 + FHBufferOffset

// U8 Current gear
#define Gear 307 + FHBufferOffset

// S8 Steering input (-127 = full left, 0 = center, 127 = full right)
#define Steer 308 + FHBufferOffset

// S8 Normalized driving line position (-127 to 127)
#define NormalDrivingLine 309 + FHBufferOffset

// S8 Normalized AI braking difference (-127 to 127)
#define NormalAiBrakeDifference 310 + FHBufferOffset