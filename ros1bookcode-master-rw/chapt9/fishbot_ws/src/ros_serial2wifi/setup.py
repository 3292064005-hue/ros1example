from distutils.core import setup
from catkin_pkg.python_setup import generate_distutils_setup

setup_args = generate_distutils_setup(
    packages=['ros_serail2wifi'],
    package_dir={'': '.'},
)

setup(**setup_args)
