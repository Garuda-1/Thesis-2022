select json_agg(min_proof_size), name
from experiments_bands
where name like '%ERX 1%' or name like '%ERX+ 1%'--name like '[6]%' or name like '[7]%'
group by name
order by name;

update experiments
set name = '[6] MCP-ERX 1 URQ'
where name = '[6] MCP-ERXX 1 URQ'

select json_agg(distinct(min_proof_size)), name
from experiments_bands
where name like '%Baseline%'
group by name
order by name;