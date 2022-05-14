select json_agg(EAER.min_proof_size_eaer) as eaer,
       json_agg(MCA.min_proof_size_mca) as mca,
       json_agg(MCPER.min_proof_size_mcper) as mcper,
       json_agg(MCPERPLUS.min_proof_size_mcperplus) as mcperplus
from (
         select
             -- EAER_SUB.proof_size,
             MIN(EAER_SUB.proof_size) over (order by EAER_SUB.timestamp) as min_proof_size_eaer,
             -- extract(epoch from (EAER_SUB.timestamp - (LAG(EAER_SUB.timestamp) over (order by EAER_SUB.timestamp)))) as duration_sec,
             row_number() over () as row_number
         from (
                  select
                      r2.proof_size,
                      r2.timestamp
                  from runs r2
                  where r2.experiment_id in (
                      select e2.id
                      from experiments e2
                               inner join runs r2 on e2.id = r2.experiment_id
                      where e2.name = '[6] EA-ER Steiner'
                      order by r2.proof_size asc
                      limit 1
                  )
                  order by r2.timestamp
              ) EAER_SUB
     ) EAER
         inner join (
    select
        -- MCA_SUB.proof_size,
        MIN(MCA_SUB.proof_size) over (order by MCA_SUB.timestamp) as min_proof_size_mca,
        -- extract(epoch from (MCA_SUB.timestamp - (LAG(MCA_SUB.timestamp) over (order by MCA_SUB.timestamp)))) as duration_sec,
        row_number() over () as row_number
    from (
             select
                 r2.proof_size,
                 r2.timestamp
             from runs r2
             where r2.experiment_id in (
                 select e2.id
                 from experiments e2
                          inner join runs r2 on e2.id = r2.experiment_id
                 where e2.name = '[6] MC-A Steiner'
                 order by r2.proof_size asc
                 limit 1
             )
             order by r2.timestamp
         ) MCA_SUB
) MCA on EAER.row_number = MCA.row_number
         inner join (
    select
        -- MCPERPLUS_SUB.proof_size,
        MIN(MCPERPLUS_SUB.proof_size) over (order by MCPERPLUS_SUB.timestamp) as min_proof_size_mcperplus,
        -- extract(epoch from (MCPERPLUS_SUB.timestamp - (LAG(MCPERPLUS_SUB.timestamp) over (order by MCPERPLUS_SUB.timestamp)))) as duration_sec,
        row_number() over () as row_number
    from (
             select
                 r2.proof_size,
                 r2.timestamp
             from runs r2
             where r2.experiment_id in (
                 select e2.id
                 from experiments e2
                 where e2.name = '[6] MCP-ER+ 1 Steiner'
                 limit 1
             )
             order by r2.timestamp
         ) MCPERPLUS_SUB
) MCPERPLUS on EAER.row_number = MCPERPLUS.row_number
         inner join (
    select
        -- MCPER_SUB.proof_size,
        MIN(MCPER_SUB.proof_size) over (order by MCPER_SUB.timestamp) as min_proof_size_mcper,
        -- extract(epoch from (MCPER_SUB.timestamp - (LAG(MCPER_SUB.timestamp) over (order by MCPER_SUB.timestamp)))) as duration_sec,
        row_number() over () as row_number
    from (
             select
                 r2.proof_size,
                 r2.timestamp
             from runs r2
             where r2.experiment_id in (
                 select e2.id
                 from experiments e2
                 where e2.name = '[6] MCP-ER 1 Steiner'
                 limit 1
             )
             order by r2.timestamp
         ) MCPER_SUB
) MCPER on EAER.row_number = MCPER.row_number
