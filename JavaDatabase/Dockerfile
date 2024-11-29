FROM maven:3.9.9 AS build
WORKDIR /app
COPY pom.xml .
RUN cat pom.xml
COPY src ./src
RUN ls -R
RUN mvn -version
RUN mvn validate
RUN mvn clean compile package -B -X -e

FROM openjdk:19-jdk-slim
WORKDIR /app
COPY --from=build /app/target/database-1.0-SNAPSHOT-jar-with-dependencies.jar app.jar
EXPOSE 8000
CMD ["java", "-jar", "app.jar"]
