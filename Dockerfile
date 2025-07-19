FROM debian:bookworm AS build

RUN apt-get update && apt-get install -y \
    build-essential \
    g++ \
    make \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY . .

RUN make

FROM debian:bookworm-slim

COPY --from=build /app/bin/mitre /usr/local/bin/mitre

CMD ["/usr/local/bin/mitre"]
