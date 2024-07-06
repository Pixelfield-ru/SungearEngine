//
// Created by ilya on 24.06.24.
//

#ifndef SUNGEARENGINE_ANNOTATIONS_H
#define SUNGEARENGINE_ANNOTATIONS_H

#define sg_struct(args, ...)
#define sg_component(args, ...)
#define sg_system(args, ...)

// USE FOR VARIABLES THAT MUST BE SERIALIZED
#define sg_member(args, ...)
#define sg_serializable(args, ...)
// USE FOR CUSTOM SPECIALIZATIONS OF SGCore::SerializerSpec
#define sg_serializer_spec(args, ...)

#endif //SUNGEARENGINE_ANNOTATIONS_H
