#include "kb.h"
#include "kb_defines.h"
#include "testdata.h"
#include "kb_debug.h"

static char str_buffer[2048];
static uint8_t fv_arr[MAX_VECTOR_SIZE];
#if SML_PROFILER
float recent_fv_times[MAX_VECTOR_SIZE];
unsigned int recent_fv_cycles[MAX_VECTOR_SIZE];
#endif

void sml_output_results(int model_index, int model_result)
{
    //bool feature_vectors = true;
    //int size = 0;
    kb_print_model_result(model_index, model_result, str_buffer, 1, fv_arr);
    printf("%s\r\n", str_buffer);
    #if SML_PROFILER
    memset(str_buffer, 0, 2048);
    kb_print_model_cycles(model_index, model_result, str_buffer, recent_fv_cycles);
    printf("%s\r\n", str_buffer);
    memset(str_buffer, 0, 2048);
    kb_print_model_times(model_index, model_result, str_buffer, recent_fv_times);
    printf("%s\r\n", str_buffer);
    #endif

};


int sml_recognition_run(int16_t *data, int batch_sz, uint8_t num_sensors)
{
    int ret = 0;
    int index = 0;
    SENSOR_DATA_T * pData;

        for(index = 0; index < batch_sz; index++)
        {
        	pData = (SENSOR_DATA_T*)&data[index];

			ret = kb_run_model((SENSOR_DATA_T *)pData, num_sensors, 0);
			if (ret >= 0)
			{
				sml_output_results(0, ret);
				kb_reset_model(0);
			};

        }
    return(ret);
}
