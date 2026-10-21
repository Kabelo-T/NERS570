#!/bin/bash

#SBATCH --job-name=example_job
#SBATCH --mail-user=kabelo@umich.edu
#SBATCH --mail-type=BEGIN,END
#SBATCH --cpus-per-task=1
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --mem-per-cpu=1000m
#SBATCH --time=05:00
#SBATCH --account=ners570f26_class
#SBATCH --partition=debug
#SBATCH --output=%x-%j.stdout

/bin/hostname
printenv
