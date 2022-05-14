select json_agg(min_proof_size), name
from experiments_bands
where name like '[6]%' or name like '[7]%'
group by name
order by name;

select json_agg(distinct(min_proof_size)), name
from experiments_bands
where name like '%Baseline%'
group by name
order by name;