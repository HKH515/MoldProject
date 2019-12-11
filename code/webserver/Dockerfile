FROM python:3.7-alpine
WORKDIR /code/webserver
ENV FLASK_APP app.py
ENV FLASK_RUN_HOST 0.0.0.0
ENV POSTGRES_URL="127.0.0.1:5432"
ENV POSTGRES_USER="postgres"
ENV POSTGRES_PW="BtQ&8DKY9#76"
ENV POSTGRES_DB="walldo"


RUN apk add --no-cache gcc musl-dev linux-headers
COPY requirements.txt requirements.txt
RUN pip install -r requirements.txt
COPY . .
CMD ["flask", "run"]
