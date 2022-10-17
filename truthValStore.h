#include <string>
#include <iostream>
using namespace std;

class TruthValStore {
    private:
        string _atomArray = "";
        int _truthValArray = 0b0; 
        
    public:
        /**
         * @brief Construct a new Truth Val Store object
         * 
         * @param atomArray 
         * @param truthValArray 
         */
        TruthValStore(string atomArray, int truthValArray) {
            _atomArray = atomArray;
            _truthValArray = truthValArray;
        }

        TruthValStore(string atomArray) {
            _atomArray = atomArray;
        }

        /**
         * @brief Set the Atom Array object
         * 
         * @param atomArray 
         */
        void setAtomArray(string atomArray) {
            _atomArray = atomArray;
        }

        /**
         * @brief Get the Atom Array object
         * 
         * @return string 
         */
        string getAtomArray() {
            return _atomArray;
        }

        /**
         * @brief Get the truth value of corresoponding atom in O(n)
         * 
         * @param atom 
         * @return true 
         * @return false 
         */
        bool getTruthVal(char atom) {
            // Get index of atom in atom array
            int index = _atomArray.find(atom);

            // All cases where atom is not 

            // Return the nth element of truthValArray from the right where n = index
            // Cheeky bitwise operation
            return (_truthValArray & (1 << index)) >> index;
        }

        /**
         * @brief Set the truth value of corresoponding atom in O(n)
         * 
         * @param atom 
         * @param truthVal 
         * @return true 
         * @return false 
         */
        void setTruthVal(char atom, bool truthVal) {
            // Get index of atom in array
            int index = _atomArray.find(atom);

            // Make mask to help replace
            int mask = 1 << index;

            // Set value in truthValArray using bitwise magic
            _truthValArray = (_truthValArray & ~mask) | ((truthVal << index) & mask);
        }


};