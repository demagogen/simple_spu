#ifndef SPU_DEFINE_UTILS_H_
#define SPU_DEFINE_UTILS_H_

#define VALIDATE_REGISTER(reg)                                       \
    if (reg > RegistersQuantityConst || reg < 0)                   \
    {                                                                \
        printf("%d\n", reg);                                         \
        return SPU_INVALID_REGISTER;                                 \
    }                                                                \

#define spu_dump(spuInfo) spu_dump_(spuInfo, __FILE__, __LINE__, __func__);

#endif
