# hello_space

cat > ~/spaceros_ws/src/hello_space/README.md <<'EOF'
# hello_space

A small Space ROS (ROS 2) package I built to learn the platform: a talker, a listener with a heartbeat watchdog, and unit tests.

## What's here
- `talker` publishes a message every second on `/chatter`, logging both system and steady clock time.
- `listener` subscribes and runs a watchdog: it warns on late messages and raises an error if the talker goes silent for more than 1.5 s.
- `gap_checker.hpp` holds the gap logic, covered by gtest unit tests.

## Investigation: the 30-second hiccups
Talker logs showed a gap every ~30 messages. I logged the steady (monotonic) clock beside each system timestamp. The steady clock advanced exactly 1.000 s every message, while the system clock stepped forward every ~30 s.

- Root cause: WSL periodically syncing its clock with Windows. The timer never stalled.
- Design consequence: the watchdog measures time with the steady clock, so clock corrections can't cause false alarms or hide real stalls.
- Lesson: a 50 ms jump detector found nothing; 5 ms caught every step. Threshold choice matters.

## Build, run, test
Inside the `osrf/space-ros` container, in a workspace:

    colcon build --packages-select hello_space
    source install/setup.bash
    ros2 run hello_space talker      # terminal 1
    ros2 run hello_space listener    # terminal 2
    colcon test --packages-select hello_space && colcon test-result --verbose

## Known issue
`ros2 pkg create` fails in the `osrf/space-ros` image (`'EntryPoints' object has no attribute 'get'` in `ament_copyright`), so this package was written by hand.
EOF
