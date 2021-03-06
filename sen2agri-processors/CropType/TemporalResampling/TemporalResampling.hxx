/*=========================================================================
  *
  * Program:      Sen2agri-Processors
  * Language:     C++
  * Copyright:    2015-2016, CS Romania, office@c-s.ro
  * See COPYRIGHT file for details.
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.

 =========================================================================*/
 
#ifndef TEMPORALRESAMPLING_HXX
#define TEMPORALRESAMPLING_HXX

#include "itkBinaryFunctorImageFilter.h"
#include "otbVectorImage.h"

#include <cmath>

#define NOVALUEPIXEL -10000.0

typedef std::vector<int> RasterDates;

struct Indices
{
    int minLo;
    int maxLo;
    int minHi;
    int maxHi;
};

struct SensorData
{
    // the name of the sensor (for information only)
    std::string sensorName;
    // the dates of the input
    RasterDates inDates;
    std::vector<int> outDates;
};

template <typename PixelType, typename MaskType>
class GapFillingFunctor
{
public:
    GapFillingFunctor() : radius(0), bands(0)
    {
    }
    GapFillingFunctor(std::vector<SensorData> &inData, int r, int b)
        : inputData(inData), radius(r), bands(b)
    {
        radius = 365000;

        outputSize = 0;
        for (const SensorData &sd : inputData) {
            outputSize += sd.outDates.size();
        }
    }

    PixelType operator()(PixelType pix, MaskType mask) const
    {
        // Create the output pixel
        PixelType result(outputSize * bands);

        int outPixelId = 0;
        int offset = 0;
        for (const auto &sd : inputData) {
            for (auto outDate : sd.outDates) {
                Indices ind = { 0, 0, 0, 0 };
                // get the indices intervals
                ind = getDateIndices(sd, outDate);

                // get the id of the first valid date before or equal to the output date
                int beforeId = getPixelDateIndex(ind.maxLo, ind.minLo, mask, offset);

                // get the id of the first valid date after or equal to the output date
                int afterId = getPixelDateIndex(ind.minHi, ind.maxHi, mask, offset);

                // build the offseted ids
                int beforeRasterId = beforeId + offset;
                int afterRasterId = afterId + offset;

                // for each band
                for (int band = 0; band < bands; band++) {

                    if (beforeId == -1 && afterId == -1) {
                        // if no valid pixel then set a default value (0.0)
                        result[outPixelId] = NOVALUEPIXEL;
                    } else if (beforeId == -1) {
                        // use only the after value
                        result[outPixelId] = pix[afterRasterId * bands + band];
                    } else if (afterId == -1) {
                        // use only the before value
                        result[outPixelId] = pix[beforeRasterId * bands + band];
                    } else if (beforeId == afterId) {
                        // use only the before value which is equal to the after value
                        result[outPixelId] = pix[beforeRasterId * bands + band];
                    } else {
                        // use linear interpolation to compute the pixel value
                        float x1 = sd.inDates[beforeId];
                        float y1 = pix[beforeRasterId * bands + band];
                        float x2 = sd.inDates[afterId];
                        float y2 = pix[afterRasterId * bands + band];
                        float a = (y1 - y2) / (x1 - x2);
                        float b = y1 - a * x1;

                        result[outPixelId] = static_cast<typename PixelType::ValueType>(a * outDate + b);
                    }
                    outPixelId++;
                }
            }
            offset += sd.inDates.size();
        }

        return result;
    }

    bool operator!=(const GapFillingFunctor a) const
    {
        return (this->radius != a.radius) || (this->bands != a.bands);
    }

    bool operator==(const GapFillingFunctor a) const
    {
        return !(*this != a);
    }

protected:
    // input data
    std::vector<SensorData> inputData;
    // the radius for date search
    int radius;
    // the number of bands per image
    int bands;
    // the number of output size in days
    int outputSize;

private:
    // get the indices of the input dates which are before and after the centerDate and within the
    // radius.
    Indices
    getDateIndices(const SensorData &sd, int centerDate) const
    {
        Indices indices = { -1, -1, -1, -1 };

        for (int i = 0; i < sd.inDates.size(); i++) {
            // get input date
            int inputDate = sd.inDates[i];

            if (inputDate > centerDate + radius) {
                // all future dates are after the center date.
                break;
            }

            if (inputDate < centerDate - radius) {
                // the date is before the interval
                continue;
            }

            // The first time this point is reached will offer the index for the minimum value of
            // the minLo value
            if (indices.minLo == -1) {
                indices.minLo = i;
            }

            if (inputDate == centerDate) {
                // this date is at least the end of the before interval and the begining of the
                // after interval
                indices.maxLo = i;
                indices.minHi = i;
                indices.maxHi = i;
            } else if (inputDate < centerDate) {
                // this is a candidate maxLo Date
                indices.maxLo = i;
            } else {
                // this can be the minHi if not already set in the previous step (at date equality)
                if (indices.minHi == -1) {
                    indices.minHi = i;
                }

                // this is a candidate maxHi
                indices.maxHi = i;
            }
        }

        return indices;
    }

    // Get the date index of the first valid pixel or -1 if none found
    int getPixelDateIndex(int startIndex, int endIndex, const PixelType &mask, int offset)
        const
    {

        if (startIndex == -1 || endIndex == -1) {
            return -1;
        }

        int index = startIndex;
        bool done = false;

        while (!done) {
            // if the pixel is masked then continue
            if (mask[index + offset] != 0) {
                // if it was the last index then stop and return -1
                if (index == endIndex) {
                    index = -1;
                    done = true;
                } else if (startIndex < endIndex) {
                    index++;
                } else {
                    index--;
                }

            } else {
                // the search is done
                done = true;
            }
        }

        return index;
    }
};

/** Binary functor image filter which produces a vector image with a
* number of bands different from the input images */
template<typename ImageType, typename Functor>
class ITK_EXPORT BinaryFunctorImageFilterWithNBands
    : public itk::BinaryFunctorImageFilter<ImageType,
                                           ImageType,
                                           ImageType,
                                           Functor >
{
public:
    typedef BinaryFunctorImageFilterWithNBands Self;
    typedef itk::BinaryFunctorImageFilter<ImageType,
                                          ImageType,
                                          ImageType,
                                          Functor > Superclass;
    typedef itk::SmartPointer<Self> Pointer;
    typedef itk::SmartPointer<const Self> ConstPointer;

    /** Method for creation through the object factory. */
    itkNewMacro(Self);

    /** Macro defining the type*/
    itkTypeMacro(BinaryFunctorImageFilterWithNBands, SuperClass);

    /** Accessors for the number of bands*/
    itkSetMacro(NumberOfOutputBands, unsigned int);
    itkGetConstMacro(NumberOfOutputBands, unsigned int);

protected:
    BinaryFunctorImageFilterWithNBands()
    {
    }
    virtual ~BinaryFunctorImageFilterWithNBands()
    {
    }

    void GenerateOutputInformation()
    {
        Superclass::GenerateOutputInformation();
        this->GetOutput()->SetNumberOfComponentsPerPixel(m_NumberOfOutputBands);
    }

private:
    BinaryFunctorImageFilterWithNBands(const Self &); // purposely not implemented
    void operator=(const Self &); // purposely not implemented

    unsigned int m_NumberOfOutputBands;
};

#endif // TEMPORALRESAMPLING_HXX
