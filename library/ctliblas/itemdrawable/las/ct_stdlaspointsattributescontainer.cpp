#include "ct_stdlaspointsattributescontainer.h"

void CT_StdLASPointsAttributesContainer::insertPointsAttributesAt(CT_LasDefine::LASPointAttributesType key, const CT_AbstractPointAttributesScalar *att)
{
    if(att != nullptr)
        internalInsertAttributes(key, att);
}

QList<CT_LasDefine::LASPointAttributesType> CT_StdLASPointsAttributesContainer::lasPointAttributesTypes() const
{
    QList<CT_LasDefine::LASPointAttributesType> l;
    QList<int> la = internalAttributes().uniqueKeys();

    foreach (int i, la) {
        l.append(static_cast<CT_LasDefine::LASPointAttributesType>(i));
    }

    return l;
}

void CT_StdLASPointsAttributesContainer::getLASDataAt(const size_t i, CT_LASData &data) const
{
    CT_AbstractPointsAttributes* attribute;

    if ((attribute = pointsAttributesAt(CT_LasDefine::Intensity)) != nullptr) {data._Intensity = quint16(static_cast<CT_AbstractPointAttributesScalar*>(attribute)->dValueAt(i));} else {data._Intensity = 0;}
    if ((attribute = pointsAttributesAt(CT_LasDefine::Return_Number)) != nullptr) {data._Return_Number = quint16(static_cast<CT_AbstractPointAttributesScalar*>(attribute)->dValueAt(i));} else {data._Return_Number = 0;}
    if ((attribute = pointsAttributesAt(CT_LasDefine::Number_of_Returns)) != nullptr) {data._Number_of_Returns = quint16(static_cast<CT_AbstractPointAttributesScalar*>(attribute)->dValueAt(i));} else {data._Number_of_Returns = 0;}
    if ((attribute = pointsAttributesAt(CT_LasDefine::Classification_Flags)) != nullptr) {data._Classification_Flags = quint16(static_cast<CT_AbstractPointAttributesScalar*>(attribute)->dValueAt(i));} else {data._Classification_Flags = 0;}
    if ((attribute = pointsAttributesAt(CT_LasDefine::Scanner_Channel)) != nullptr) {data._Scanner_Channel = quint16(static_cast<CT_AbstractPointAttributesScalar*>(attribute)->dValueAt(i));} else {data._Scanner_Channel = 0;}
    if ((attribute = pointsAttributesAt(CT_LasDefine::Scan_Direction_Flag)) != nullptr) {data._Scan_Direction_Flag = quint16(static_cast<CT_AbstractPointAttributesScalar*>(attribute)->dValueAt(i));} else {data._Scan_Direction_Flag = 0;}
    if ((attribute = pointsAttributesAt(CT_LasDefine::Edge_of_Flight_Line)) != nullptr) {data._Edge_of_Flight_Line = quint16(static_cast<CT_AbstractPointAttributesScalar*>(attribute)->dValueAt(i));} else {data._Edge_of_Flight_Line = 0;}
    if ((attribute = pointsAttributesAt(CT_LasDefine::Classification)) != nullptr) {data._Classification = quint8(static_cast<CT_AbstractPointAttributesScalar*>(attribute)->dValueAt(i));} else {data._Classification = 0;}
    if ((attribute = pointsAttributesAt(CT_LasDefine::Scan_Angle_Rank)) != nullptr) {data._Scan_Angle_Rank = qint16(static_cast<CT_AbstractPointAttributesScalar*>(attribute)->dValueAt(i));} else {data._Scan_Angle_Rank = 0;}
    if ((attribute = pointsAttributesAt(CT_LasDefine::User_Data)) != nullptr) {data._User_Data = quint8(static_cast<CT_AbstractPointAttributesScalar*>(attribute)->dValueAt(i));} else {data._User_Data = 0;}
    if ((attribute = pointsAttributesAt(CT_LasDefine::Point_Source_ID)) != nullptr) {data._Point_Source_ID = quint16(static_cast<CT_AbstractPointAttributesScalar*>(attribute)->dValueAt(i));} else {data._Point_Source_ID = 0;}
    if ((attribute = pointsAttributesAt(CT_LasDefine::GPS_Time)) != nullptr) {data._GPS_Time = quint16(static_cast<CT_AbstractPointAttributesScalar*>(attribute)->dValueAt(i));} else {data._GPS_Time = 0;}
    if ((attribute = pointsAttributesAt(CT_LasDefine::Red)) != nullptr) {data._Red = quint16(static_cast<CT_AbstractPointAttributesScalar*>(attribute)->dValueAt(i));} else {data._Red = 0;}
    if ((attribute = pointsAttributesAt(CT_LasDefine::Green)) != nullptr) {data._Green = quint16(static_cast<CT_AbstractPointAttributesScalar*>(attribute)->dValueAt(i));} else {data._Green = 0;}
    if ((attribute = pointsAttributesAt(CT_LasDefine::Blue)) != nullptr) {data._Blue = quint16(static_cast<CT_AbstractPointAttributesScalar*>(attribute)->dValueAt(i));} else {data._Blue = 0;}
    if ((attribute = pointsAttributesAt(CT_LasDefine::NIR)) != nullptr) {data._NIR = quint16(static_cast<CT_AbstractPointAttributesScalar*>(attribute)->dValueAt(i));} else {data._NIR = 0;}
    if ((attribute = pointsAttributesAt(CT_LasDefine::Wave_Packet_Descriptor_Index)) != nullptr) {data._Wave_Packet_Descriptor_Index = quint8(static_cast<CT_AbstractPointAttributesScalar*>(attribute)->dValueAt(i));} else {data._Wave_Packet_Descriptor_Index = 0;}
    if ((attribute = pointsAttributesAt(CT_LasDefine::Byte_offset_to_waveform_data)) != nullptr) {data._Byte_offset_to_waveform_data = quint16(static_cast<CT_AbstractPointAttributesScalar*>(attribute)->dValueAt(i));} else {data._Byte_offset_to_waveform_data = 0;}
    if ((attribute = pointsAttributesAt(CT_LasDefine::Waveform_packet_size_in_bytes)) != nullptr) {data._Waveform_packet_size_in_bytes = quint16(static_cast<CT_AbstractPointAttributesScalar*>(attribute)->dValueAt(i));} else {data._Waveform_packet_size_in_bytes = 0;}
    if ((attribute = pointsAttributesAt(CT_LasDefine::Return_Point_Waveform_Location)) != nullptr) {data._Return_Point_Waveform_Location = quint16(static_cast<CT_AbstractPointAttributesScalar*>(attribute)->dValueAt(i));} else {data._Return_Point_Waveform_Location = 0;}
}

QHash<CT_LasDefine::LASPointAttributesType, CT_AbstractPointAttributesScalar *> CT_StdLASPointsAttributesContainer::lasPointsAttributes() const
{
    QHash<CT_LasDefine::LASPointAttributesType, CT_AbstractPointAttributesScalar *> h;
    QHash<int, CT_AbstractPointsAttributes *> ha = internalAttributes();

    QHashIterator<int, CT_AbstractPointsAttributes *> it(ha);

    while(it.hasNext()) {
        it.next();
        h.insert(static_cast<CT_LasDefine::LASPointAttributesType>(it.key()), static_cast<CT_AbstractPointAttributesScalar*>(it.value()));
    }

    return h;
}
