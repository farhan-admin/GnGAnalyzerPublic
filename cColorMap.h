#pragma once
#include <cmath>
#include <QWidget>
#include <QPainter>
#include <QVector>
#include <QDebug>

#include "cerrwarninfo.h"

/// <summary>
/// Structure to store, add, subtract, mulitply, divide and assign color values
/// </summary>
struct strctColor 
{

public:
    strctColor():
        m_red(0),
        m_green(0),
        m_blue(0),
        m_alpha(0)
    {}

    strctColor(unsigned char red, unsigned char green, unsigned char blue) :
        m_red(red),
        m_green(green),
        m_blue(blue),
        m_alpha(255)
    {}

    strctColor(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha):
        m_red(red),
        m_green(green),
        m_blue(blue),
        m_alpha(alpha)
    {}

    strctColor operator-(const strctColor& rhs)
    {
        strctColor lhs;

        lhs.m_red = this->m_red - rhs.m_red;
        lhs.m_green = this->m_green - rhs.m_green;
        lhs.m_blue = this->m_blue - rhs.m_blue;
        lhs.m_alpha = this->m_alpha - rhs.m_alpha;

        return lhs;
    }

    strctColor operator+(const strctColor& rhs)
    {
        strctColor lhs;
        lhs.m_red = this->m_red + rhs.m_red;
        lhs.m_green = this->m_green + rhs.m_green;
        lhs.m_blue = this->m_blue + rhs.m_blue;
        lhs.m_alpha = this->m_alpha + rhs.m_alpha;

        return lhs;
    }

    strctColor operator/(const uint& rhs)
    {
        strctColor lhs;

        lhs.m_red = this->m_red / rhs;
        lhs.m_green = this->m_green / rhs;
        lhs.m_blue = this->m_blue / rhs;
        lhs.m_alpha = this->m_alpha / rhs;

        return lhs;
    }

    strctColor operator/(const strctColor& rhs)
    {
        strctColor lhs;

        lhs.m_red = this->m_red / rhs.m_red;
        lhs.m_green = this->m_green / rhs.m_green;
        lhs.m_blue = this->m_blue / rhs.m_blue;
        lhs.m_alpha = this->m_alpha / rhs.m_alpha;

        return lhs;
    }

    strctColor operator*(const uint& rhs)
    {
        strctColor lhs;

        lhs.m_red = this->m_red / rhs;
        lhs.m_green = this->m_green / rhs;
        lhs.m_blue = this->m_blue / rhs;
        lhs.m_alpha = this->m_alpha / rhs;

        return lhs;
    }

    strctColor operator*(const strctColor& rhs)
    {
        strctColor lhs;

        lhs.m_red = this->m_red / rhs.m_red;
        lhs.m_green = this->m_green / rhs.m_green;
        lhs.m_blue = this->m_blue / rhs.m_blue;
        lhs.m_alpha = this->m_alpha / rhs.m_alpha;

        return lhs;
    }

    void operator=(const strctColor& rhs)
    {
        this->m_red = rhs.m_red;
        this->m_green = rhs.m_green;
        this->m_blue = rhs.m_blue;
        this->m_alpha = rhs.m_alpha;
    }

    int m_red;
    int m_green;
    int m_blue;
    int m_alpha;
};

//Using template class, in case strctColor type does not work and I need to swithch to the QVector or QColor types 
//template<typename  T>
class cColorMap
{

public:

    cColorMap() :
        m_isColorMapInit(false),
        m_MaxNegAmp(-1.0),
        m_MaxPosAmp(1.0),
        m_AmpRate(1.0),
        m_isDivrgColrMap(true),
        m_numColrs(0){}

    /// Generates a DIVERGING colormap.
    /// In diverging ColorMap, colors are expanding outwards from a center color
    /// Commonly used in displaying seismic data.
    bool GeneDivergingColorMap(strctColor& maxPosColr, strctColor& maxNegColr, strctColor& zeroColr, uint& nColr)
    {
        //Storing the variables in the class members, because these will be passed on to the fragment shader
        m_MaxPosColr = maxPosColr;
        m_MaxNegColr = maxNegColr;
        m_ZeroColr = zeroColr;
        m_numColrs = nColr;

        unsigned char ALPHA = 255; //Keeping Alpha constant for first version of this software

        if (m_numColrs > 0)
        {
            /// If an odd number is provided it will get incremented by one to get to the next even number.
            /// So that Positive and Negative amplitudes have equal number of colors
            if ( (m_numColrs % 2) != 0 )
                ++m_numColrs;

            m_vecUByteDivergingColrMap.reserve(m_numColrs * 4); // 4 color components, RGBA

            float halfSpctrm = m_numColrs / 2;

            /// For Negative Half of the Color Map
            float deltaR =  ( m_ZeroColr.m_red - m_MaxNegColr.m_red) / halfSpctrm ;
            float deltaG =  ( m_ZeroColr.m_green - m_MaxNegColr.m_green) / halfSpctrm ;
            float deltaB =  ( m_ZeroColr.m_blue - m_MaxNegColr.m_blue) / halfSpctrm ;

            unsigned char  r = ( m_MaxNegColr.m_red);
            unsigned char  g = ( m_MaxNegColr.m_green);
            unsigned char  b = ( m_MaxNegColr.m_blue);
                
            for (uint i = 0; i < m_numColrs/2; ++i)
            {
                m_vecUByteDivergingColrMap.push_back(r);
                m_vecUByteDivergingColrMap.push_back(g);
                m_vecUByteDivergingColrMap.push_back(b);
                m_vecUByteDivergingColrMap.push_back(ALPHA); //for alpha channel
                r = std::round(r + deltaR);
                g = std::round(g + deltaG);
                b = std::round(b + deltaB);
            }

            /// For Positive Half of the Color Map
            deltaR = ( m_MaxPosColr.m_red   - m_ZeroColr.m_red  ) / halfSpctrm;
            deltaG = ( m_MaxPosColr.m_green - m_ZeroColr.m_green) / halfSpctrm;
            deltaB = ( m_MaxPosColr.m_blue  - m_ZeroColr.m_blue ) / halfSpctrm;

            r = m_ZeroColr.m_red;
            g = m_ZeroColr.m_green;
            b = m_ZeroColr.m_blue;
                
            for (uint i = 0; i < m_numColrs/2; ++i)
            {
                m_vecUByteDivergingColrMap.push_back(r);
                m_vecUByteDivergingColrMap.push_back(g);
                m_vecUByteDivergingColrMap.push_back(b);
                m_vecUByteDivergingColrMap.push_back(ALPHA); //for alpha channel
                r = std::round( r + deltaR );
                g = std::round( g + deltaG );
                b = std::round( b + deltaB );
            }

            m_isColorMapInit = true;
            m_isDivrgColrMap = true;

            return true;
         }

        else
        {
            m_isColorMapInit = false;
            return false;
        }
    }

    /// Generates a LINEAR colormap.
    /// In Linear ColorMap, colors are increasing from Minimum to Maximum linearly
    /// Commonly used in displaying geophysical properties like Velocity, Density etc.
    bool GeneLinearColorMap(strctColor& maxPosColr, strctColor& maxNegColr, uint& nColr)
    {
        //Storing the variables in the class members, because these will be passed on to the fragment shader
        m_MaxPosColr = maxPosColr;
        m_MaxNegColr = maxNegColr;
        m_numColrs = nColr;

        unsigned char ALPHA = 255; //Keeping Alpha constant for first version for this software

        if (m_numColrs > 0)
        {
            /// If an odd number is provided it will get incremented by one to get to the next even number.
              if ((m_numColrs % 2) != 0)
                ++m_numColrs;

            m_vecUByteLinearColrMap.reserve(m_numColrs * 4); // 4 color components, RGBA

            float diff = 0.0;

            diff = m_MaxPosColr.m_red - m_MaxNegColr.m_red;
            float deltaR = diff / m_numColrs;

            diff = m_MaxPosColr.m_green - m_MaxNegColr.m_green;
            float deltaG = diff / m_numColrs;

            diff = m_MaxPosColr.m_blue - m_MaxNegColr.m_blue;
            float deltaB = diff/ m_numColrs;

            unsigned char  r = (m_MaxNegColr.m_red);
            unsigned char  g = (m_MaxNegColr.m_green);
            unsigned char  b = (m_MaxNegColr.m_blue);

            for (uint i = 0; i < m_numColrs; ++i)
            {
                m_vecUByteLinearColrMap.push_back(r);
                m_vecUByteLinearColrMap.push_back(g);
                m_vecUByteLinearColrMap.push_back(b);
                m_vecUByteLinearColrMap.push_back(ALPHA); //for alpha channel
                r = std::round(std::abs( r + deltaR));
                g = std::round(std::abs( g + deltaG));
                b = std::round(std::abs( b + deltaB));
            }

            
            m_isColorMapInit = true;
            m_isDivrgColrMap = false;

            //QString str;
            //for (int i = 0; i < m_vecUByteLinearColrMap.size() - 4; i += 4) {
            //    str = QString::number(m_vecUByteLinearColrMap[i]) + "," + QString::number(m_vecUByteLinearColrMap[i + 1]) + "," + QString::number(m_vecUByteLinearColrMap[i + 2]) + "," + QString::number(m_vecUByteLinearColrMap[i + 3]);
            //    cErrWarnInfo::EWI(ewiINFOR, str);
            //}

            return true;

        }

        else
        {
            m_isColorMapInit = false;
            return false;
        }
    }

    /// Returns a gradient object, that can be displayed as a color bar.
    /// Example taken from
    /// https://stackoverflow.com/questions/50611762/how-to-use-qt-qcolormap-in-code/50612579
    /// 
    std::shared_ptr<QLinearGradient> GetColorBar()
    {
        QVector<unsigned char>* ptrColrMap = 0;

        if (m_isDivrgColrMap)
            ptrColrMap = &m_vecUByteDivergingColrMap;
        else
            ptrColrMap = &m_vecUByteLinearColrMap;

        
        if (m_isColorMapInit)
        {
            std::shared_ptr<QLinearGradient> gradient = std::make_shared<QLinearGradient>(0, 0, m_numColrs, 0);

            QGradientStops stops;
            qreal pos = 0.0;

            for (int i = 0; (i + 4) <= ptrColrMap->size(); i += 4 )
            {
                stops << QGradientStop( (pos)/m_numColrs , qRgba(ptrColrMap->at(i), ptrColrMap->at(i + 1), ptrColrMap->at(i + 2), ptrColrMap->at(i + 3)));
                ++pos;
            }

            gradient->setStops(stops);

            return gradient;
        }

        else
            return nullptr;
    }
    
    /// <summary>
    /// This function looks up the equivalent color in RGB for the provided valAmp. It updates three unsigned char values in the provided vector with RGB color.
    /// 
    /// Before calling this function, the Color Map should already be initialized.
    /// 
    /// Default Min and Max Amplitude range is -1.0 and 1.0. You can update this range by calling the function UpdateAmpRange(qreal& maxNegAmp, qreal& maxPosAmp).
    /// Updating Min Max amplitude will not update the Color Map.
    /// 
    /// Data can be normalized (means divided by a constant) by the NormFact
    /// </summary>
    bool GetColor(QVector<float>& seisSamp, std::shared_ptr< QVector<unsigned char>> vecOutRGB, float& NormFact)
    {
        if (m_isDivrgColrMap)
            GetDivrgColor(seisSamp, vecOutRGB, NormFact);
        else
            GetLinearColor(seisSamp, vecOutRGB, NormFact);

        return false;
    }

    bool GetDivrgColor(QVector<float>& seisSamp, std::shared_ptr< QVector<unsigned char>> vecOutRGB, float& NormFact)
    {
        if (!m_isColorMapInit)
            return false;

        if (NormFact == 0)
            NormFact = 1;

        vecOutRGB->reserve(seisSamp.size() * 4);

        for (auto valAmp : seisSamp)
        {
            valAmp *= NormFact;

            //For Zero Amplitudes. Hint: There are a lot of Zeros in seismic data
            if (valAmp == 0.0)
            {
                vecOutRGB->push_back(m_ZeroColr.m_red);  // R
                vecOutRGB->push_back(m_ZeroColr.m_green);// G
                vecOutRGB->push_back(m_ZeroColr.m_blue); // B
                vecOutRGB->push_back(m_ZeroColr.m_alpha); // Alpha
                continue;
            }

            if (valAmp < m_MaxNegAmp)
            {
                vecOutRGB->push_back(m_MaxNegColr.m_red); // R
                vecOutRGB->push_back(m_MaxNegColr.m_green); // G
                vecOutRGB->push_back(m_MaxNegColr.m_blue); // B
                vecOutRGB->push_back(m_MaxNegColr.m_alpha); // Alpha
                continue;
            }

            if (valAmp > m_MaxPosAmp)
            {
                vecOutRGB->push_back(m_MaxPosColr.m_red); // R
                vecOutRGB->push_back(m_MaxPosColr.m_green); // G
                vecOutRGB->push_back(m_MaxPosColr.m_blue);  // B
                vecOutRGB->push_back(m_MaxPosColr.m_alpha); // Alpha
                continue;
            }

            if (valAmp < 0)
            {
                //Color Map's Zero index corresponds to the m_MaxNegColr
                quint32 indx = std::round( std::abs((m_MaxNegAmp - valAmp)/m_AmpRate ) ); //m_MaxNegAmp and valAmp are negative numbers, so subtracting the two to get the difference
                indx = indx * 4; //times 4 because each sample is converted to 4 unsigned chars 

                if ((indx + 3) >= m_vecUByteDivergingColrMap.size())
                    indx = m_vecUByteDivergingColrMap.size() - 4; /// Sample values that are just a fraction short of the max amplitude,
                                    /// will get the color of the maximum amplitude

                vecOutRGB->push_back(m_vecUByteDivergingColrMap[indx]); // R
                vecOutRGB->push_back(m_vecUByteDivergingColrMap[indx + 1]); // G
                vecOutRGB->push_back(m_vecUByteDivergingColrMap[indx + 2]);  // B
                vecOutRGB->push_back(m_vecUByteDivergingColrMap[indx + 3]); // Alpha
                                
                continue;
            }

            if (valAmp > 0)
            {
                quint32 indx = std::round( std::abs(valAmp/m_AmpRate ) ) + (m_numColrs/2) + 1; //m_MaxNegAmp is a negative number so adding a positive number to it gives the difference
                indx = indx * 4; //times 4 because each sample is converted to 4 unsigned chars  

                if ((indx + 3) >= (quint32)m_vecUByteDivergingColrMap.size())
                    indx = m_vecUByteDivergingColrMap.size() - 4; /// Sample values that are just a fraction short of the max amplitude,
                                    /// will get the color of the maximum amplitude
                                    
                vecOutRGB->push_back(m_vecUByteDivergingColrMap[indx]); // R
                vecOutRGB->push_back(m_vecUByteDivergingColrMap[indx + 1]); // G
                vecOutRGB->push_back(m_vecUByteDivergingColrMap[indx + 2]);  // B 
                vecOutRGB->push_back(m_vecUByteDivergingColrMap[indx + 3]); // Alpha

                continue;
            }
        }

        //Included these lines because some samples were not accepted in any of the above if else statements
       // Q_ASSERT((seisSamp.size() * sizeof(float)) == (vecOutRGB->size()));

      if ((seisSamp.size() * sizeof(float)) != (vecOutRGB->size()))
      {
            cErrWarnInfo::EWI(ewiERROR, "Not all seismic sample could be converted to color values.");
      }

        return false;
    }

    bool GetLinearColor(QVector<float>& seisSamp, std::shared_ptr< QVector<unsigned char>> vecOutRGB, float& NormFact)
    {
        if (!m_isColorMapInit)
            return false;

        if (NormFact == 0)
            NormFact = 1;

        vecOutRGB->reserve(seisSamp.size() * 4);

        int indx;
        float valAmp = 0.0;

        for (int s = 0; s < seisSamp.size(); ++s)
        {
            valAmp = seisSamp[s];

            valAmp *= NormFact;

            if (valAmp <= m_MaxNegAmp)
            {
                vecOutRGB->push_back(m_MaxNegColr.m_red); // R
                vecOutRGB->push_back(m_MaxNegColr.m_green); // G
                vecOutRGB->push_back(m_MaxNegColr.m_blue); // B
                vecOutRGB->push_back(m_MaxNegColr.m_alpha); // Alpha
                continue;
            }

            if (valAmp >= m_MaxPosAmp)
            {
                vecOutRGB->push_back(m_MaxPosColr.m_red); // R
                vecOutRGB->push_back(m_MaxPosColr.m_green); // G
                vecOutRGB->push_back(m_MaxPosColr.m_blue);  // B
                vecOutRGB->push_back(m_MaxPosColr.m_alpha); // Alpha
                continue;
            }


            indx = std::round(abs((m_MaxNegAmp - valAmp) / m_AmpRate));
            indx = indx * 4; //times 4 because each color comprises of 4 channels.

            if ((indx + 3) >= m_vecUByteLinearColrMap.size())
                indx = indx - 4; /// Sample values that are just a fraction short of the max amplitude,
                                 /// will get the color of the maximum amplitude

            vecOutRGB->push_back(m_vecUByteLinearColrMap[indx]); // R
            vecOutRGB->push_back(m_vecUByteLinearColrMap[indx + 1]); // G
            vecOutRGB->push_back(m_vecUByteLinearColrMap[indx + 2]);  // B
            vecOutRGB->push_back(m_vecUByteLinearColrMap[indx + 3]); // Alpha

        }

        //Included these lines because some samples were not accepted in any of the above if else statements
        //Q_ASSERT((seisSamp.size() * sizeof(float)) == (vecOutRGB->size()));

        if ((int)(seisSamp.size() * sizeof(float)) != (vecOutRGB->size()))
        {
            cErrWarnInfo::EWI(ewiERROR, "Not all seismic sample could be converted to color values.");
        }

        return false;
    }

    bool IsInitialized() { return m_isColorMapInit;}
    strctColor GetNegColrRate() { return m_v3NegColrRate; }
    strctColor GetPosColrRate() { return m_v3PosColrRate; }
    strctColor GetMaxNegColr() { return m_MaxNegColr; }
    strctColor GetMaxPosColr() { return m_MaxPosColr; }
    strctColor GetZeroColr() { return m_ZeroColr; }
    void UpdateAmpRange(float& maxNegAmp, float& maxPosAmp) {
        m_MaxNegAmp = maxNegAmp;
        m_MaxPosAmp = maxPosAmp;
        m_AmpRate = abs ( (m_MaxNegAmp - m_MaxPosAmp) / m_numColrs ) ;
    };
    float GetMaxNegAmp() { return m_MaxNegAmp; }
    float GetMaxPosAmp() { return m_MaxPosAmp; }
    uint GetNumOfColrs() { return m_numColrs; }

private:
    //Stores rgba channels of the color map
    //std::shared_ptr<QVector<float>> m_ptrVecColrMap = std::make_shared<QVector<float>>();
    QVector<unsigned char> m_vecUByteDivergingColrMap; //GLubyte LUT
    QVector<unsigned char> m_vecUByteLinearColrMap; //GLubyte LUT
    bool m_isColorMapInit; //Only proceed with color lookup, if this is true
    strctColor m_v3NegColrRate; //Rate of change for NEGATIVE amplitudes
    strctColor m_v3PosColrRate; //Rate of change for POSITIVE amplitudes
    strctColor m_MaxNegColr; //Color for the Maximum Negative amplitude
    strctColor m_ZeroColr; // Color for the Zero amplitudes
    strctColor m_MaxPosColr; //Color for the Maximum Positive amplitude
    float m_MaxNegAmp;  //The Maximum Negative amplitude for this color map
    float m_MaxPosAmp;  //The Maximum Positive amplitude for this color map
    float m_AmpRate;
    bool m_isDivrgColrMap; // Attribute controls if a diverging colormap was created or a Linear color map
    uint m_numColrs; //Number of colors in this color map. (m_numColrs / 2) gives colors of the negative & positive amplitudes
};

//cColorMap<strctColor>;

