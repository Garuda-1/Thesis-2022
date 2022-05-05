CREATE TABLE experiments
(
    id         BIGSERIAL PRIMARY KEY,
    name       VARCHAR NOT NULL,
    start_time TIMESTAMP DEFAULT NOW(),
    end_time   TIMESTAMP
);

CREATE TABLE runs
(
    id            BIGSERIAL PRIMARY KEY,
    proof_size    BIGINT    NOT NULL,
    experiment_id BIGINT    NOT NULL REFERENCES experiments,
    timestamp     TIMESTAMP NOT NULL DEFAULT NOW()
);

CREATE OR REPLACE VIEW experiments_bands AS
SELECT E.id,
       E.name,
       E.start_time,
       E.end_time,
       R.min_proof_size,
       R.max_proof_size,
       R.runs_count,
       R.last_run_timestamp
FROM experiments E
         INNER JOIN (
    SELECT RS.experiment_id,
           MIN(RS.proof_size) as min_proof_size,
           MAX(RS.proof_size) as max_proof_size,
           MAX(RS.timestamp)  as last_run_timestamp,
           COUNT(*)           as runs_count
    FROM runs RS
    GROUP BY RS.experiment_id
) R ON E.id = R.experiment_id;

CREATE OR REPLACE VIEW memory_consumption AS
SELECT pg_database.datname                                 AS database_name,
       pg_database_size(pg_database.datname) / 1024 / 1024 AS size_mb
FROM pg_database
ORDER by pg_database_size(pg_database.datname) DESC;