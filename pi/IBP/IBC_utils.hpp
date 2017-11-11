#ifndef IBC_UTILS_HPP
#     define IBC_UTILS_HPP

namespace IBC
{

//TODO add more different ways to extract the true size of the package
//from different views
uint8_t truesize(uint8_t id);

uint8_t hash8(void * data, uint8_t size);

bool IBC_dynamic (uint8_t id);

}

#endif /* IBC_UTILS_HPP */

