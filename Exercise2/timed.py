import time


def timed(prompt: str):
    def decorator(func):
        def inner(*args, **kwargs):
            print(f"\nStarting procedure: {prompt}")
            start = time.perf_counter()
            rv = func(*args, **kwargs)
            end = time.perf_counter()
            print(f"Process `{prompt}` finished after {end - start} seconds\n")
            return rv
        return inner
    return decorator
