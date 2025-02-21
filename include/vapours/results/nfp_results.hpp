/*
 * Copyright (c) 2018-2020 Atmosphère-NX
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once
#include <vapours/results/results_common.hpp>

namespace ams::nfp {

    R_DEFINE_NAMESPACE_RESULT_MODULE(115);

    R_DEFINE_ERROR_RESULT(DeviceNotFound,                  64);
    R_DEFINE_ERROR_RESULT(InvalidArgument,                 65);
    R_DEFINE_ERROR_RESULT(WrongApplicationAreaSize,        68);
    R_DEFINE_ERROR_RESULT(WrongDeviceState,                73);
    R_DEFINE_ERROR_RESULT(NfcDisabled,                     80);
    R_DEFINE_ERROR_RESULT(WriteAmiiboFailed,               88);
    R_DEFINE_ERROR_RESULT(TagRemoved,                      97);

    R_DEFINE_ERROR_RESULT(RegistrationIsNotInitialized,    120);
    R_DEFINE_ERROR_RESULT(ApplicationAreaIsNotInitialized, 128);
    R_DEFINE_ERROR_RESULT(CorruptedDataWithBackup,         136);
    R_DEFINE_ERROR_RESULT(CorruptedData,                   144);

    R_DEFINE_ERROR_RESULT(WrongApplicationAreaId,          152);
    R_DEFINE_ERROR_RESULT(ApplicationAreaExist,            168);
    R_DEFINE_ERROR_RESULT(NotAnAmiibo,                     178);
    R_DEFINE_ERROR_RESULT(UnableToAccessBackupFile,        200);

}
