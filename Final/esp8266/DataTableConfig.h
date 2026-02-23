/**
 * @file    DataTableConfig.h
 * @brief   Database Config header file
 *
 * The Database config is used to create an table to configure the datatable objects' data types and initial values.
 *
 * 
 */

#ifndef __DATATABLE_CONFIG_H__
#define __DATATABLE_CONFIG_H__

#include <stddef.h>
#include <stdlib.h>
#include "ComponentCommonDefinitions.h"
#include "DataTable.h"
#include "DataTags.h"

/**
 * @defgroup DataTableConfigVariables 2.2.3 List of Database Config Variables
 * @defgroup DataTableConfigFunctions 2.2.4 List of Database Config Functions
 */

/**
 * @brief Fills the Database Table with testing data
 * @ingroup DataTableConfigFunctions
 *
 * @param[out] datatable Database object address to store the found config object (if any was found)
 * @return OK If the DataTable was properly configured, NOK Otherwise.
 */
uint8_t config_datatable(data_object_st *datatable);

#endif  // __DATATABLE_CONFIG_H__
