import time
from typing import Callable

import numpy as np


def profile_function(n_trials: int, func: Callable, args: tuple, kwargs: dict = {}) -> np.ndarray:
    runtimes = []
    for _ in range(n_trials):
        start = time.time()
        _ = func(*args, **kwargs)
        runtimes.append(time.time() - start)

    return np.array(runtimes)


def pretty_print_title(title: str):
    title_length = len(title)
    hbar = '-' * title_length

    print(
        '\n'
        f'{title}\n'
        f'{hbar}'
    )


def pretty_print_statistics(runtimes: np.ndarray):
    print(
        f'    Average: {runtimes.mean()}\n'
        f'    Standard deviation: {runtimes.std()}\n'
        f'    Median: {np.median(runtimes)}\n'
        f'    Min: {np.min(runtimes)}\n'
        f'    Max: {np.max(runtimes)}\n'
    )
