import time


def timed(prompt: str):
    def decorator(func):
        def inner(*args, **kwargs):
            print(f"\nStarting procedure: {prompt}")
            start = time.perf_counter()
            rv = func(*args, **kwargs)
            end = time.perf_counter()
            print(f"Finished after {end - start} seconds")
            return rv
        return inner
    return decorator
