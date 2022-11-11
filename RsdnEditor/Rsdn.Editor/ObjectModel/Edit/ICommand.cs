using System;
using System.Collections.Generic;
using System.Text;

namespace Rsdn.Editor.ObjectModel.Edit
{
	/// <summary>
	/// ������� ������������� ������.
	/// </summary>
	public interface ICommand
	{
		/// <summary>
		/// ��������� ������ �� �������������� ������ ���������. ���������� 
		/// ���������� ������� (���������� ����� ������� ������ ����� ������ 
		/// ����� ��������� � �������� ���������).
		/// </summary>
		/// <returns>
		/// ���������� ������� (�� ���������� �������� ��������� ���������
		/// ������ ��������, ��� ����� ������������� ����� ���������).</returns>
		ICommand Execute();

		ICommand Execute(Range<Document> previousSelection);

		/// <summary>
		/// �������� � �������� ����������� �������������.
		/// </summary>
		Document Document { get; }

		/// <summary>
		/// ������ ��������� �� ���������� �������.
		/// </summary>
		Range<Document> SelectionBefor { get; }
		/// <summary>
		/// ������ ��������� ����� ���������� �������.
		/// </summary>
		Range<Document> SelectionAfter { get; }
	}
}
