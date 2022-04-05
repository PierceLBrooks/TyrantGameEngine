/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

#ifndef TGE_RANDOM_HPP
#define TGE_RANDOM_HPP

/*************************************/
/**             Headers             **/
/*************************************/
#include <random>

namespace TGE
{
    class RNG
    {
        public:
            RNG()
            {
                gen.seed(rd());
            }

            int geni(int min, int max)
            {
                std::uniform_int_distribution<int> dist(min, max);
                return dist(gen);
            }

            float genf(float min, float max)
            {
                std::uniform_real_distribution<float> dist(min, max);
                return dist(gen);
            }

            double gend(double min, double max)
            {
                std::uniform_real_distribution<double> dist(min, max);
                return dist(gen);
            }

            template<class Sseq>
            void seed(Sseq& seq)
            {
                gen.seed(seq);
            }

            void seed()
            {
                gen.seed(rd());
            }

        protected:
            std::random_device rd;
            std::mt19937 gen;
    };
}

#endif // TGE_RANDOM_HPP
