import numpy as np


def generate_test_points(n: int = 1_000, size: int = 2, min_val: float = -100, max_val: float = 100) -> np.ndarray:
    return np.random.uniform(min_val, max_val, (n, size))
