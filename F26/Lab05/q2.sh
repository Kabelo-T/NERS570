#!/usr/bin/bash
#SBATCH --job-name=q2
#SBATCH --account=ners570f26_class
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task=1
#SBATCH --mem-per-cpu=90G
#SBATCH --time=02:00:00

module load gcc/15.1.0 openmpi/5.0.10

EXE=/gpfs/accounts/ners570f26_class_root/ners570f26_class/kabelo/petsc/share/petsc/examples/src/ksp/ksp/tutorials/ex15

# GMRES with no restart (restart=10000) runs as its own job in q2_gmres_norestart.sh
for ksp_type in cg bcgs gmres
do
    if [[ $ksp_type == "gmres" ]]; then
        for rstart in 30 200
        do
            for dim in 8 32 128 256 512 1024
            do
                echo -e " ${dim} x ${dim} ${ksp_type} restart = ${rstart}\n"
                time mpiexec -np 1 ${EXE} -pc_type none -m ${dim} -n ${dim} \
                -ksp_type $ksp_type \
                -ksp_max_it 10000 -ksp_gmres_restart $rstart \
                -ksp_converged_reason
                echo
            done
        done
    else
        for dim in 8 32 128 256 512 1024
        do
            echo -e " ${dim} x ${dim} ${ksp_type}\n"
            time mpiexec -np 1 ${EXE} -pc_type none -m ${dim} -n ${dim} \
            -ksp_type $ksp_type \
            -ksp_converged_reason
            echo
        done
    fi
done > q2_timing.txt 2>&1
