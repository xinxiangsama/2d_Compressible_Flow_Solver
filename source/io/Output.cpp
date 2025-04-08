#include "Output.h"

Output::Output(Run *run)
{
    m_run = run;
}

void Output::WriteToHDF5(const std::string& filename)
{   
    auto N1local = m_run->m_mesh->getnumberCellsX();
    auto N2local = m_run->m_mesh->getnumberCellsY();
    auto N1global = m_run->m_mesh->getnumberCellsXGlobal();
    auto N2global = m_run->m_mesh->getnumberCellsYGlobal();
    auto offsetX = m_run->m_mesh->getoffsetX();
    auto offsetY = m_run->m_mesh->getoffsetY();

    H5::FileAccPropList fapl;
    H5Pset_fapl_mpio(fapl.getId(), MPI_COMM_WORLD, MPI_INFO_NULL);

    H5::H5File file(filename, H5F_ACC_TRUNC, H5::FileCreatPropList::DEFAULT, fapl);
    hsize_t global_dims[2] = {N1global, N2global};
    hsize_t local_dims[2] = {N1local, N2local};
    hsize_t offset[2] = {offsetX, offsetY};

    H5::DataSpace global_dataspace(2, global_dims);
    H5::DataSpace local_dataspace(2, local_dims);
    global_dataspace.selectHyperslab(H5S_SELECT_SET, local_dims, offset);

    H5::DSetCreatPropList plist;
    plist.setChunk(2, local_dims);

    H5::DataSet dataset_Mu = file.createDataSet("Mu", H5::PredType::NATIVE_DOUBLE, global_dataspace, plist);
    H5::DataSet dataset_Mv = file.createDataSet("Mv", H5::PredType::NATIVE_DOUBLE, global_dataspace, plist);
    H5::DataSet dataset_Rho = file.createDataSet("Rho", H5::PredType::NATIVE_DOUBLE, global_dataspace, plist);
    H5::DataSet dataset_Energy = file.createDataSet("Energy", H5::PredType::NATIVE_DOUBLE, global_dataspace, plist);
    H5::DataSet dataset_Pressure = file.createDataSet("Pressure", H5::PredType::NATIVE_DOUBLE, global_dataspace, plist);
    H5::DataSet dataset_FluxMu = file.createDataSet("FluxMu", H5::PredType::NATIVE_DOUBLE, global_dataspace, plist);
    H5::DataSet dataset_FluxMv = file.createDataSet("FluxMv", H5::PredType::NATIVE_DOUBLE, global_dataspace, plist);
    H5::DataSet dataset_FluxRho = file.createDataSet("FluxRho", H5::PredType::NATIVE_DOUBLE, global_dataspace, plist);
    H5::DataSet dataset_FluxEnergy = file.createDataSet("FluxEnergy", H5::PredType::NATIVE_DOUBLE, global_dataspace, plist);
    H5::DataSet dataset_Udir = file.createDataSet("Udir", H5::PredType::NATIVE_DOUBLE, global_dataspace, plist);
    H5::DataSet dataset_Vdir = file.createDataSet("Vdir", H5::PredType::NATIVE_DOUBLE, global_dataspace, plist);
    H5::DataSet dataset_Tdir = file.createDataSet("Tdir", H5::PredType::NATIVE_DOUBLE, global_dataspace, plist);

    std::vector<double> Mu_local(N1local * N2local, 0.0);
    std::vector<double> Mv_local(N1local * N2local, 0.0);
    std::vector<double> Rho_local(N1local * N2local, 0.0);
    std::vector<double> Energy_local(N1local * N2local, 0.0);
    std::vector<double> FluxMu_local(N1local * N2local, 0.0);
    std::vector<double> FluxMv_local(N1local * N2local, 0.0);
    std::vector<double> FluxRho_local(N1local * N2local, 0.0);
    std::vector<double> FluxEnergy_local(N1local * N2local, 0.0);
    std::vector<double> Pressure_local(N1local * N2local, 0.0);
    std::vector<double> Udir_local(N1local * N2local, 0.0);
    std::vector<double> Vdir_local(N1local * N2local, 0.0);
    std::vector<double> Tdir_local(N1local * N2local, 0.0);

    for (int i = 0; i < N1local; ++i) {
        for (int j = 0; j < N2local; ++j) {
            int idx = i * N2local + j;
            auto& cell = m_run->m_cellsLv1[i][j];
            Mu_local[idx] = cell.getPhase()->getU();
            Mv_local[idx] = cell.getPhase()->getV();
            Rho_local[idx] = cell.getPhase()->getDensity();
            Energy_local[idx] = cell.getPhase()->getTotalEnergy();
            FluxRho_local[idx] = cell.getFlux()->getMass();
            FluxMu_local[idx] = cell.getFlux()->getMomentum()[0];
            FluxMv_local[idx] = cell.getFlux()->getMomentum()[1];
            FluxEnergy_local[idx] = cell.getFlux()->getEnergy();
            Pressure_local[idx] = cell.getPhase()->getPressure(); // Add pressure
            Udir_local[idx] = cell.getGradient()->getUGradient()[0];
            Vdir_local[idx] = cell.getGradient()->getVGradient()[1];
            Tdir_local[idx] = cell.getGradient()->getTGradient()[0];
        }
    }

    dataset_Mu.write(Mu_local.data(), H5::PredType::NATIVE_DOUBLE, local_dataspace, global_dataspace);
    dataset_Mv.write(Mv_local.data(), H5::PredType::NATIVE_DOUBLE, local_dataspace, global_dataspace);
    dataset_Rho.write(Rho_local.data(), H5::PredType::NATIVE_DOUBLE, local_dataspace, global_dataspace);
    dataset_Energy.write(Energy_local.data(), H5::PredType::NATIVE_DOUBLE, local_dataspace, global_dataspace);
    dataset_FluxMu.write(FluxMu_local.data(), H5::PredType::NATIVE_DOUBLE, local_dataspace, global_dataspace);
    dataset_FluxMv.write(FluxMv_local.data(), H5::PredType::NATIVE_DOUBLE, local_dataspace, global_dataspace);
    dataset_FluxRho.write(FluxRho_local.data(), H5::PredType::NATIVE_DOUBLE, local_dataspace, global_dataspace);
    dataset_FluxEnergy.write(FluxEnergy_local.data(), H5::PredType::NATIVE_DOUBLE, local_dataspace, global_dataspace);
    dataset_Pressure.write(Pressure_local.data(), H5::PredType::NATIVE_DOUBLE, local_dataspace, global_dataspace); // Write pressure
    dataset_Udir.write(Udir_local.data(), H5::PredType::NATIVE_DOUBLE, local_dataspace, global_dataspace);
    dataset_Vdir.write(Vdir_local.data(), H5::PredType::NATIVE_DOUBLE, local_dataspace, global_dataspace);
    dataset_Tdir.write(Tdir_local.data(), H5::PredType::NATIVE_DOUBLE, local_dataspace, global_dataspace);
}