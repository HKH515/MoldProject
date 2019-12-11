FROM python:3.7-alpine
WORKDIR /code/webserver
ENV FLASK_APP app.py
ENV FLASK_RUN_HOST webserver
ENV FLASK_ENV development
#ENV POSTGRES_URL="0.0.0.0:2345"
#ENV POSTGRES_USER="postgres"
#ENV POSTGRES_PASSWORD="BtQ&8DKY9#76"
#ENV POSTGRES_DB="walldo"


RUN apk add --no-cache gcc musl-dev linux-headers
RUN apk update && apk add postgresql-dev gcc python3-dev musl-dev
#RUN apk add wait-for-it


COPY requirements.txt requirements.txt
RUN pip install -r requirements.txt
COPY . .
#CMD ["./wait-for-it.sh", "database:5432", "--", "flask", "run"]
CMD ["flask", "run"]
