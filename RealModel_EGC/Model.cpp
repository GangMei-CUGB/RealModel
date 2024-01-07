#include "StdAfx.h"
#include "Model.h"
#include <fstream>

CModel::CModel(void)
{
}


CModel::~CModel(void)
{
}

void CModel::Clear(void)
{
	for (int i = m_aBlks.GetUpperBound(); i >= 0; i--) {
		m_aBlks.GetAt(i)->Clear();
		delete m_aBlks.GetAt(i);  
		m_aBlks.RemoveAt(i);
	}

	for (int i = m_aPhds.GetUpperBound(); i >= 0; i--) {
		m_aPhds.GetAt(i)->Clear();
		delete m_aPhds.GetAt(i);  
		m_aPhds.RemoveAt(i);
	}

	for (int i = m_aLps.GetUpperBound(); i >= 0; i--) {
		m_aLps.GetAt(i)->Clear();
		delete m_aLps.GetAt(i);  
		m_aLps.RemoveAt(i);
	}

	for (int i = m_aNds.GetUpperBound(); i >= 0; i--) {
		m_aNds.GetAt(i)->Clear();
		delete m_aNds.GetAt(i);  
		m_aNds.RemoveAt(i);
	}

	for (int i = m_aSurfs.GetUpperBound(); i >= 0; i--) {
		m_aSurfs.GetAt(i)->Clear();
		delete m_aSurfs.GetAt(i);  
		m_aSurfs.RemoveAt(i);
	}

}