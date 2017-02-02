# BPP-category-conflicts-optimizer
BPP with conflicts between cargo types

This program finds an optimum solution of the Bin Packing Problem with conflicts between cargo types, using Gurobi (license required!)

Instances were created based on Delorme et al. (2016) -> we adopted 6 cargo types

Input files:
- instances' names are given by ArquivosIntancias-saidas.txt (edit it to run more/less instances, or to change input files paths)
- instances' data are given by files on folders Instancias and InstanciasNR
- compatibility between categories is given by Reduced_set.vrp (1 indicates items compatibility and 0 otherwise)
