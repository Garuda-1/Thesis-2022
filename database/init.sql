CREATE TABLE experiments
(
    id         BIGSERIAL PRIMARY KEY,
    name       VARCHAR NOT NULL,
    start_time TIMESTAMP NOT NULL DEFAULT NOW(),
    end_time   TIMESTAMP
);

CREATE TABLE runs
(
    id            BIGSERIAL PRIMARY KEY,
    proof_size    BIGINT NOT NULL,
    experiment_id BIGINT NOT NULL REFERENCES experiments,
    timestamp     TIMESTAMP NOT NULL DEFAULT NOW()
);