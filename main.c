#include <stdio.h>
#include <stdint.h>

typedef struct _posit32 {
    /** implement your code **/
    uint32_t val;

} posit;

int sizeOfPosit = 8;
int es = 2;// максимум 4 иначе не влезет
int useed = 16;

/**
 * 128 - 10000000
 * @param position from 0 till sizeOfPosit to get the bit on specific position
 * @param number posit type for which we want the bit
 * @return the bit for specified number
 */
uint32_t getBitOnPosition(int position, posit number){
    return ((number.val << position) & 128) >> (sizeOfPosit - 1);
}

/**
 * Function that returns sign of the posit
 * the sign is the first bit of the number
 * @param num posit to work with
 * @return sign of the posit
 */
uint32_t getSign(posit num){
    return num.val >> (sizeOfPosit - 1);
}

/**
 * Gets the firstbit of the regime which is on the 1 position of the number (from 0)
 * Iterates over every bit in the number till the next bit will not be equal to the current bit
 * @param num posit type that we want to work with
 * @return the size of the regime without the last bit
 */
uint32_t getRegimeSize(posit num){
    uint32_t firstbit = getBitOnPosition(1, num);
    int currentShift = 2;
    int sizeOfRegime = 1;
    while(getBitOnPosition(currentShift++, num) == firstbit){
        sizeOfRegime++;
    }
    return sizeOfRegime;
}

/**
 * es at most might be only 2, but in case if it is not true this function should be rewritten
 * es + 1 -> in case if it is 2 in binary it will become 11; in case if it is 1 it will be still only one 1
 * @return the mask for the exponent
 */
uint32_t getExponentMask(){
    int temp = es;
    if(es == 2){
        temp++;
    }
    return (temp << (sizeOfPosit - es));
}

/**
 * Returns size of exponent
 * By now it is very silly function
 */
uint32_t getExponentSize(){
    return es;
}


/**
 * First gets the size of the regime then gets the first number of exponent
 * Shifts the number and make AND operation with the result of shift
 * Shit the result number to most right position to get the result number = exponent
 * @param num with what we will work
 * @return value of exponent
 */
uint32_t getExponent(posit num){
    uint32_t regimeSize = getRegimeSize(num) + 1;

    uint32_t first_num = (num.val << (regimeSize + 1));

    return (first_num & getExponentMask()) >> (sizeOfPosit - getExponentSize());
}

/**
 * Generating mask from int because it is very convenient in this case
 * example:
 * in: 3; out: 111;
 * in: 2; out: 11;
 * @param number - value to generate
 * @return mask in format 11
 */
uint32_t getMaskFromInt(int number){
    uint32_t result = 0;
    for(int i = 0; i < number; i++){
        result = result << 1;
        result |= 1;
    }
    return result;
}

uint32_t getFractionSize(posit num){
    return sizeOfPosit - getExponentSize() - 1 /*size of sign*/ - getRegimeSize(num) - 1;
}

/**
 * Take all that left -> it will be fraction
 * @param num
 * @return
 */
uint32_t getFraction(posit num){
    int sizeOfFraction = sizeOfPosit - getExponentSize() - 1 /*size of sign*/ - getRegimeSize(num) - 1;
    return num.val & getMaskFromInt(sizeOfFraction);
}

int intPow(int base, int p){
    if(p != 0) {
        return intPow(base * base, p - 1);
    }else{
        return base;
    }
}


uint32_t getRegime(posit num){
    uint32_t firstBit = getBitOnPosition(1, num);
    uint32_t regimeSize = getRegimeSize(num);

    if(firstBit == 1){
        return regimeSize - 1;
    }else{//if 0
        return -regimeSize;
    }
}

/**
 * TODO проверить сортировку двух чисел по убыванию
 * @return
 */
posit addition(posit valueA, posit valueB){
    posit a = valueA;
    posit b = valueB;
    if(valueA.val < valueB.val) {
        posit a = valueB;
        posit b = valueA;
    }

    uint32_t a_frac = (1 << getFractionSize(a)) | getFraction(a);
    uint32_t b_frac = (1 << getFractionSize(b)) | getFraction(b);

    uint32_t regimeLeft = getRegime(a);
    uint32_t regimeRight = getRegime(b);

    uint32_t expLeft = getExponent(a);
    uint32_t expRight = getExponent(b);
    
    uint32_t howMuchToShift = (regimeLeft - regimeRight) * intPow(2, es) + (expLeft - expRight);

    uint32_t shiftedFraction = b_frac >> howMuchToShift;

    uint32_t resultFraction = (shiftedFraction + a_frac) & getMaskFromInt(3);

    posit result = {(a.val & (getMaskFromInt(5) << 3)) | resultFraction};
    return result;
}


/**
 * TODO проверить нужно ли сортировать
 *
 * @param valueA
 * @param valueB
 * @return
 */
posit multiplication (posit valueA, posit valueB){
    posit a = valueA;
    posit b = valueB;
    if(valueA.val < valueB.val) {
        posit a = valueB;
        posit b = valueA;
    }


}

int main() {
    posit a = {72};
    posit b = {68};

    printf("Addition: %d\n", addition(a, b));

    return 0;
}