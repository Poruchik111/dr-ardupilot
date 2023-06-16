# EAMS Robotics wind turbine2 mount control script

EAMS Robotics wind turbine2 gimbal mount control script

How to use
    Connect 360 lidar (e.g. RPLidarS1, SF45b) to one of the autopilot's serial ports
    Set SERIALx_PROTOCOL = 11 (Lidar360) where "x" corresponds to the serial port connected to the lidar
    Set PRX1_TYPE = 5 (RPLidar) or 8 (SF45b)
    Set OA_TYPE = 2 (Dijkstras)
    Set OA_DB_BEAM_WIDTH = 1 so lidar beam width of 1 deg
    Set OA_DB_DIST_MAX = 20 so objects ignored beyond 20m
    Set OA_DB_RADIUS_MIN = 0.1 so all objects are at least 0.1m
    Set OA_DB_OUTPUT = 3 to send all objects to GCS
    Set SCR_ENABLE = 1 to enable scripting and reboot the autopilot
    Copy this script to the autopilot's APM/scripts directory