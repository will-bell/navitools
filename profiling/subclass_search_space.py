import numpy as np
from navitools import SearchSpace

from common import plot_roadmap


class FooSearchSpace(SearchSpace):

    def __init__(self):
        super().__init__(2)

    def sample_free_space(self, n: int):
        return np.random.uniform(0, 1, (n, 2))

    def valid_transition(self, a: np.ndarray, b: np.ndarray) -> bool:
        return True

    def transition_cost(self, a: np.ndarray, b: np.ndarray) -> float:
        return np.sqrt(a.dot(b))


if __name__ == '__main__':
    from navitools import build_prm
    import matplotlib.pyplot as plt

    search_space = FooSearchSpace()
    roadmap = build_prm(100, 5, 5, search_space)

    _, ax = plt.subplots()
    plot_roadmap(ax, roadmap)
    plt.show()