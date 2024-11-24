import sys
if sys.prefix == '/usr':
    sys.real_prefix = sys.prefix
    sys.prefix = sys.exec_prefix = '/home/quinton/tritonrobotics/training/tr-autonomy-2-rxlc/src/install/spinnyrobot'
