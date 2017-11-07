import threading

def worker(num):
    print(num)

for i in range(5):
    t = threading.Thread(target=worker, args=(i, ))
    t.start()
