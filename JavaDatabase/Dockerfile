FROM openjdk:19-jdk-slim

WORKDIR /app

COPY src /app/src
COPY src /app/lib

RUN javac -cp lib/*:. src/main/java/*.java

CMD ["java", "-cp", "lib/*:src", "Main"]