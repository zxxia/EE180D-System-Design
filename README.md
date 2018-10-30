# ee180d_project

This is a motion detection program running on Intel Edison Platform. It can classify motions like walking, running, still, start ascent/descent, and etc.

Usage:

make

./producer
Run producer at 9DOF sensor so generate data files named after time stamps.

./consumer
Run consumer to view the classification results at console.
