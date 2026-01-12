<!--

 gcloud compute ssh mpplabc34n-slurm-login-001 --zone=asia-south1-b --project=mpplab-482405

mkdir -p build
cd build
cmake ..
make -j
cd ..


srun --mpi=pmi2 --nodes=2 --ntasks-per-node=1 ./build/mpi_server

squeue

scancel 82

 -->
