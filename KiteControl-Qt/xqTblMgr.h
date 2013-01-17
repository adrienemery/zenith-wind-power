/*
 * Auxiliary class helps to manage TableWidget items
 * copyright (c) 2011, fluxrobotics
 */

#ifndef H_XQTBL_MGR_H
#define H_XQTBL_MGR_H

#include <QTableWidget>
#include <QTableWidgetItem>
#include <vector>
#include <QHeaderView>

class XTableMgr
{
private:
	int m_rows;
	int m_cols;

	QTableWidget *m_tblWidget;

	std::vector<QTableWidgetItem *> m_vpItems;
	std::vector<QTableWidgetItem *> m_vpVHeaderItems;
	std::vector<QTableWidgetItem *> m_vpHHeaderItems;
public:
	XTableMgr(): m_rows(0),m_cols(0),m_tblWidget(0) { };

	~XTableMgr()
	{
		// cleanup table items
		for (int i = 0; i < m_rows*m_cols; i++)
		{
			delete m_vpItems[i];
			m_vpItems[i] = 0;
		}
		m_vpItems.resize(0);

		// cleanup horizontal header items
		for (int col = 0; col < m_cols; col++)
		{
			delete m_vpHHeaderItems[col];
			m_vpHHeaderItems[col] = 0;
		}
		m_vpHHeaderItems.resize(0);

		// cleanup vertical header items
		for (int row = 0; row < m_rows; row++)
		{
			delete m_vpVHeaderItems[row];
			m_vpVHeaderItems[row] = 0;
		}
		m_vpVHeaderItems.resize(0);

		m_tblWidget = 0;
		m_rows = m_cols = 0;
	}

	int rows() { return m_rows; }
	int cols() { return m_cols; }

	bool initTbl(QTableWidget *tblWidget,int rows_ = -1, int cols_ = -1)
	{
		m_tblWidget = tblWidget;

		if (rows_ >= 1)
			tblWidget->setRowCount(rows_);
		else
			rows_ = tblWidget->rowCount();

		if (cols_ >= 1)
			tblWidget->setColumnCount(cols_);
		else
			cols_ = tblWidget->columnCount();

		m_rows = rows_;
		m_cols = cols_;

		// Horizontal header items
		m_vpHHeaderItems.resize(m_cols);
		for (int col = 0; col < m_cols; col++)
		{
			QTableWidgetItem * pNewItem = new QTableWidgetItem();
			m_vpHHeaderItems[col] = pNewItem;
			tblWidget->setHorizontalHeaderItem(col,pNewItem);
		}
		m_tblWidget->horizontalHeader()->show();

		// Vertical header items
		m_vpVHeaderItems.resize(m_rows);
		for (int row = 0; row < m_rows; row++)
		{
			QTableWidgetItem * pNewItem = new QTableWidgetItem();
			m_vpVHeaderItems[row] = pNewItem;
			tblWidget->setVerticalHeaderItem(row,pNewItem);
		}
		m_tblWidget->verticalHeader()->show();

		// Init table items
		m_vpItems.resize(m_rows*m_cols);
		for (int row = 0; row < m_rows; row++)
		{
			for (int col = 0; col < m_cols; col++)
			{
				QTableWidgetItem * pNewItem = new QTableWidgetItem();
				m_vpItems[m_cols*row + col] = pNewItem;
				tblWidget->setItem(row,col,pNewItem);
			}
		}
		return true;
	}

	QTableWidgetItem *item(int row,int col)
	{
		if (row < m_rows && col < m_cols)
		{
			return m_vpItems[m_cols*row + col];
		}
		return NULL;
	}

	void setText(int row,int col,QString text)
	{
		item(row,col)->setText(text);
	}

	void setHHeaderText(int col, QString text)
	{
		m_vpHHeaderItems[col]->setText(text);
	}
};

#endif
