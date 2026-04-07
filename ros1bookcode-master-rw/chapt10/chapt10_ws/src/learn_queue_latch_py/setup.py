from distutils.core import setup
from catkin_pkg.python_setup import generate_distutils_setup

setup_args = generate_distutils_setup(
    packages=['learn_queue_latch_py'],
    package_dir={'': '.'},
)

setup(**setup_args)
