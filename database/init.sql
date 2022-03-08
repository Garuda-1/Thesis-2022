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
    proof_size    BIGINT NOT NULL,
    experiment_id BIGINT NOT NULL REFERENCES experiments,
    timestamp     TIMESTAMP NOT NULL DEFAULT NOW()
);

CREATE VIEW experiments_bands AS
SELECT E.id, E.name, E.start_time, E.end_time, R.min_proof_size, R.max_proof_size
FROM experiments E
INNER JOIN (
    SELECT RS.experiment_id, MIN(RS.proof_size) as min_proof_size, MAX(rs.proof_size) as max_proof_size
    FROM runs RS
    GROUP BY RS.experiment_id
) R ON E.id = R.experiment_id;