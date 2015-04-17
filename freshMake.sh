mkdir src/hexapod_bringup/include
mkdir ./holdPlz
mv src/hexapod_description ./holdPlz
mv src/hexapod_teleop_joystick ./holdPlz
mv src/joy ./holdPlz
mv src/hexapod_bringup ./holdPlz
mv src/hexapod_locomotion ./holdPlz
mv src/hexapod_sound ./holdPlz
mv src/imu_filter_madgwick ./holdPlz
mv src/phidgets_drivers ./holdPlz
catkin_make
mv holdPlz/* ./src/
catkin_make
