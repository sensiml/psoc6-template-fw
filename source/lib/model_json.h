#ifndef __MODEL_JSON_H__
#define __MODEL_JSON_H__

const char recognition_model_string_json[] = {"{\"NumModels\":1,\"ModelIndexes\":{\"0\":\"\"},\"ModelDescriptions\":[{\"Name\":\"\",\"ClassMaps\":{\"1\":\"Compact\",\"2\":\"Large\",\"3\":\"Medium\",\"0\":\"Unknown\"},\"ModelType\":\"PME\",\"FeatureFunctions\":[\"MFCC\",\"Histogram\",\"Histogram\"]}]}"};

int recognition_model_string_json_len = sizeof(recognition_model_string_json);

#endif /* __MODEL_JSON_H__ */
