[![Build](https://github.com/will-bell/navitools/actions/workflows/wheels.yml/badge.svg)](https://github.com/will-bell/navitools/actions/workflows/wheels.yml)

# Overview
Modular tools for path/motion planning in n-dimensional configuration spaces

Check out the scripts in /profiling for examples and speed statistics.

Data structures:
- K-dimensional tree
- Roadmaps

Algorithms:
- Probabilistic Roadmap
- Dijkstra search

# Installation
This package is installable from the Python Package Index for Ubuntu, MacOS, and Windows! Currently only Python >=3.6 is supported, but support for Python 2.7 is also planned for the near future.

```
pip install navitools
```

Alternatively, build the tool yourself by cloning this repository. On Windows, Microsoft Visual Studio C++ >=2015 is recommended. On MacOS and Linux, the standard gcc compiler with Ninja is recommended.

```
git clone https://github.com/will-bell/navitools.git
cd navitools
pip install .
```

If you wish to test your installation and run examples, then also clone this repository. `pytest` is used for running the unit tests, and `matplotlib` is currently used to plot the results in the examples. As with any Python project, the use of virtual environments is strongly recommended.

```
git clone https://github.com/will-bell/navitools.git

virtualenv venv       # create your virtual environment

venv\scripts\activate # if on Windows
venv/bin/activate     # if on MacOS/Linux

pip install matplotlib pytest
cd navitools
pytest tests          # run the tests

cd profiling
python polygonal_search_space.py  # run one of the profiling examples
```
