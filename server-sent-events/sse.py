import time
from flask import Flask, Response

app = Flask(__name__)

@app.route("/")
def hello():
    return "Hello Avesh!"

@app.route("/stream")
def stream():
    resp = Response(count())
    resp.headers['Content-Type'] = 'text/event-stream'
    return resp

def count():
    count = 1
    while True:
        # SSE is delimited by \n\n
        yield f"{count}\n\n"
        time.sleep(5)
        count += 1

def main():
    app.run(host="localhost", port="1289")

main()
