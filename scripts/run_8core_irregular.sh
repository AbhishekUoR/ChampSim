TRACE_DIR=/scratch/cluster/akanksha/CRCRealTraces
binary=${1}
n_warm=${2}
n_sim=${3}
num=${4}
outputdir=${5}
option=${6}

trace1=`sed -n ''$num'p' /scratch/cluster/haowu/isb-meta/ChampSim_DPC3/sim_list/8core_irregular.txt | awk '{print $1}'`
trace2=`sed -n ''$num'p' /scratch/cluster/haowu/isb-meta/ChampSim_DPC3/sim_list/8core_irregular.txt | awk '{print $2}'`
trace3=`sed -n ''$num'p' /scratch/cluster/haowu/isb-meta/ChampSim_DPC3/sim_list/8core_irregular.txt | awk '{print $3}'`
trace4=`sed -n ''$num'p' /scratch/cluster/haowu/isb-meta/ChampSim_DPC3/sim_list/8core_irregular.txt | awk '{print $4}'`
trace5=`sed -n ''$num'p' /scratch/cluster/haowu/isb-meta/ChampSim_DPC3/sim_list/8core_irregular.txt | awk '{print $5}'`
trace6=`sed -n ''$num'p' /scratch/cluster/haowu/isb-meta/ChampSim_DPC3/sim_list/8core_irregular.txt | awk '{print $6}'`
trace7=`sed -n ''$num'p' /scratch/cluster/haowu/isb-meta/ChampSim_DPC3/sim_list/8core_irregular.txt | awk '{print $7}'`
trace8=`sed -n ''$num'p' /scratch/cluster/haowu/isb-meta/ChampSim_DPC3/sim_list/8core_irregular.txt | awk '{print $8}'`

(/scratch/cluster/haowu/isb-meta/ChampSim_DPC3/bin/${binary} -warmup_instructions ${n_warm}000000 -simulation_instructions ${n_sim}000000 ${option} -hide_heartbeat -traces ${TRACE_DIR}/${trace1}.trace.gz ${TRACE_DIR}/${trace2}.trace.gz ${TRACE_DIR}/${trace3}.trace.gz ${TRACE_DIR}/${trace4}.trace.gz ${TRACE_DIR}/${trace5}.trace.gz ${TRACE_DIR}/${trace6}.trace.gz ${TRACE_DIR}/${trace7}.trace.gz ${TRACE_DIR}/${trace8}.trace.gz) &> ${outputdir}/mix${num}.txt
