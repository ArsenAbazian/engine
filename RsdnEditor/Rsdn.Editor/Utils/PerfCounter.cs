using System;
using System.Runtime.InteropServices;
using System.Diagnostics.CodeAnalysis;

	/// <summary>
	/// ��� ��������� ��������� ���������� �������� ���������� ���� ����� ��
	/// �������� ������ ��������. ���������� ���������� ������������ � ������
	/// ����������, � ����� ����������� � ���������� (���������� ����� 
	/// �������� ������ �������).
	/// </summary>
[SuppressMessage("Microsoft.Naming", "CA1704:IdentifiersShouldBeSpelledCorrectly")]
[SuppressMessage("Microsoft.Design", "CA1050:DeclareTypesInNamespaces")]
public struct PerfCounter
{
	Int64 _start;

	/// <summary>
	/// �������� ������� ������� ����������.
	/// </summary>
	public void Start()
	{
		_start = 0;
		QueryPerformanceCounter(ref _start);
	}

	/// <summary>
	/// ��������� ������� ������� ���������� � ���������� ����� � ��������.
	/// </summary>
	/// <returns>����� � �������� ���������� �� ���������� �������
	/// ����. ���������� ����� �������� ���� �������.</returns>
	public float Finish()
	{
		Int64 finish = 0;
		QueryPerformanceCounter(ref finish);

		Int64 freq = 0;
		QueryPerformanceFrequency(ref freq);
		return (((float)(finish - _start) /(float)freq));
	}

	[SuppressMessage("Microsoft.Design", "CA1060:MovePInvokesToNativeMethodsClass")]
	[DllImport("Kernel32.dll")]
	[return: MarshalAs(UnmanagedType.Bool)]
	static extern bool QueryPerformanceCounter(ref Int64 performanceCount);

	[SuppressMessage("Microsoft.Design", "CA1060:MovePInvokesToNativeMethodsClass")]
	[DllImport("Kernel32.dll")]
	[return: MarshalAs(UnmanagedType.Bool)]
	static extern bool QueryPerformanceFrequency(ref Int64 frequency);
}
