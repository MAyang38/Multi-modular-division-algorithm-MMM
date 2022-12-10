#include<cstdio>
#include<algorithm>
#include<iostream>
#include<cmath>
#include<vector>
using namespace std;

const int task_num = 10;

vector<vector<int>> init(int data[][task_num]);                                                                 //初始化函数
vector<vector<int>> Link(vector<vector<int>>& data,int num,int direction, int link_type);                       //划分算法 
vector<vector<int>> combine(vector<vector<int>> &list, vector<int> &first, vector<int>&second, int j, int i);   //合并子类
int JudgeThreshold(vector<vector<int>>& data, vector<int>& first,vector<int>& second,int distance, int link_type);  //阈值判断
void res_print(int distance, vector<vector<int>> &list);                                                            //结果输出


int getMaxDistance(vector<vector<int>> data);                                                           //计算最大距离
int calculateDistance(vector<vector<int>> data, vector<vector<int>> list);                              //计算通信代价

#define single
#define Complete
#define Average

int main()
{

    int matrix[task_num][task_num] =
    {
    {0, 1, 0, 0, 0, 0, 4, 0, 2, 0},
    {1, 0, 3, 0, 0, 0, 0, 0, 0, 0},
    {0, 3, 0, 0, 0, 0, 0, 2, 0, 1},
    {0, 0, 0, 0, 0, 2, 0, 5, 6, 0},
    {0, 0, 0, 0, 0, 0, 0, 3, 7, 8},
    {0, 0, 0, 2, 0, 0, 1, 0, 0, 0},
    {4, 0, 0, 0, 0, 1, 0, 0, 0, 0},
    {0, 0, 2, 5, 3, 0, 0, 0, 0, 0},
    {2, 0, 0, 6, 7, 0, 0, 0, 0, 0},
    {0, 0, 1, 0, 8, 0, 0, 0, 0, 0}};//导入通信代价矩阵

   

    vector<vector<int>> data = init(matrix);

    vector<vector<int>> tempMatrix = data;
    #ifdef single
        vector<vector<int>> list1=Link(data,3,4, 1);//得到经过单连接划分后的list，其中簇中元素最多为3，最多有4个簇
      
    

        int totalDistance1=calculateDistance(tempMatrix,list1);//计算划分后各簇之间的总通信距离
        cout<<"通信距离："<<totalDistance1<<endl;
    #endif
    cout<<endl;

    #ifdef Complete

        vector<vector<int>> list2=Link(data,3,4, 2);//得到经过全连接划分后的list，其中簇中元素最多为3，最多有4个簇
        int totalDistance2=calculateDistance(tempMatrix,list2);//计算划分后各簇之间的总通信距离
        cout<<"通信距离："<<totalDistance2<<endl;
    #endif


    cout<<endl;

    #ifdef Average

        vector<vector<int>> list3=Link(data, 3, 4, 3);//得到经过均值连接划分后的list，其中簇中元素最多为3，最多有4个簇
       int totalDistance3=calculateDistance(tempMatrix,list3);//计算划分后各簇之间的总通信距离
        cout<<"通信距离："<<totalDistance3<<endl;
    #endif



	return 0;
}



vector<vector<int>> init(int data[][task_num])
{
    vector<vector<int>> res;
    for(int i = 0; i < task_num; i++)
    {
        vector<int> temp;
        for(int j = 0; j < task_num; j++)
        {
            temp.push_back(data[i][j]);
        }
        res.push_back(temp);
    }
    

    return res;
}


// 划分结果输出
void res_print(int distance, vector<vector<int>>& list)
{
    cout<<distance<<"    "<< list.size()<<"    ";
    for (int i=0; i< list.size(); i++) 
    {
        vector<int> temp = list[i];
        int length = temp.size();
        if(length > 1)
            cout<<"{";
        for (int j=0;j < temp.size();j++) 
        {   
            cout<<temp[j]+1 ;//将划分后的簇输出
            if(j != temp.size() - 1)
                cout<<",";
            
        }
        if(length > 1)
            cout<<"}";
        if( i != list.size() - 1)
            cout<<",";
     
    }
 

    cout<<endl;
    

}


int getMaxDistance(vector<vector<int>> data)//计算最大距离
{
    int max=-1;
    int length=task_num;
    for (int i = 0; i < length; i++) 
    {
        for (int j = i; j < length; j++) 
        {
            if (data[i][j] >= max) 
            {
                max = data[i][j];
            }
        }
    }
    return max;
}

int calculateDistance(vector<vector<int>> data, vector<vector<int>> list)
{
    int totalDistance=0;
    //先找到两个相邻的簇，然后匹配簇中的元素
    for (int i = 0; i < list.size()-1; i++) 
    {
        vector<int> last= list[i];//得到相邻的簇
        
        for (int j=i+1;j<list.size();j++)
        {int sum = 0;
            vector<int> next=list[j];//得到相邻的簇
            for (int m=0;m<last.size();m++)//进行元素的两两匹配
            {
                
                for (int n=0;n<next.size();n++)
                {   
                    
                    if( data[last[m]][next[n]])
                    {//若两元素间有链路相链接
                        sum += data[last[m]][next[n]];
                        totalDistance+=data[last[m]][next[n]];
                    }
                    
                }
            }
        cout<< "模块"<< i + 1 <<"与模块"<< j + 1<<"通信代价:"<<sum<<endl;
        }
       
    }
    return totalDistance;
}



vector<vector<int>> Link(vector<vector<int>>& data,int num,int direction, int link_type)
{


    bool border[task_num][task_num]  = {false};//确定该边是否已划分
    bool used[task_num]  = {false} ;//确定该元素是否已放入簇中
    int length=data.size();//矩阵大小
    int number=0;//记录簇的个数
    int count=0;//计算已经放到簇中的数据个数
    vector<vector<int>> list;
    
   // data=getDistance(data);//将通信代价矩阵转换为通信距离矩阵
    int maxLength=getMaxDistance(data);//计算通信距离的最大值

    //初始化最初十个
    for(int i = 0; i < 10; i++)
    {
        vector<int> temp ;
        temp.push_back(i);
        list.push_back(temp);
    }
    int distance = maxLength + 1;
    int m = list.size();
    int MMMSAA = 0;
    while(1)
    {
        while(distance != 0 && m > 4)
        {
            int flag = 0;//是否进行下一轮
            for(int i = 0; i < list.size(); i++)
            {
                vector<int> first = list[i];
                for(int j = i + 1; j < list.size(); j++)
                {
                    vector<int> second = list[j];
                    if(JudgeThreshold(data, first, second,distance, link_type))      //如果均值大于阈值
                    {
                        if(first.size() + second.size() <= num)
                        {
                            list = combine(list, first, second, j, i);
                            flag = 1;
                        }
                    }
                    if(flag)
                        break;
                }
                if(flag)
                    break;
                
            }
           // res_print(distance, list);

            if(flag )
                m--;
            else
            {   
                if(!MMMSAA)
                    res_print(distance, list);
                    //cout<<endl<<distance<<endl;
                distance--;
            } 
            
        // 如果没有划分成四个模块，则执行MMMSAA
        }
        if(m <= 4)
        {
            if(MMMSAA)
            {   cout<<"MMMSAA链接划分结果为:  "<<endl;
                res_print(distance , list);
            }
            else
            {
                res_print(distance , list);

                if(link_type == 1)
                {   cout<<"单链接划分后得到"<<endl;
                }
                else if(link_type == 2)
                {   cout<<"全链接划分后得到"<<endl;
                }
                else if(link_type == 3)
                {   cout<<"均值链接划分后得到"<<endl;
                }
                res_print(distance , list);

            }
           break;

        }
        else
        {   
            MMMSAA = 1;
            link_type = 1;  
            distance = maxLength + 1;
        }
    }
    return list;
}





int JudgeThreshold(vector<vector<int>> &data, vector<int> &first,vector<int> &second,int distance, int link_type)
{
    int i,j;
    
    int first_num = first.size();
    int second_num = second.size();

    //单值
    if(link_type == 1)
    {
        int flag = 0;
        for(i = 0; i< first_num; i++)
        {
            for(j = 0; j < second_num; j++)
            {
                if( data[first[i]][second[j]] >= distance)
                {    
                    flag = 1;
                    break;
                }
                if(flag)
                    break;
            }
        }
        return flag;
    }
    // 全连接
    else if(link_type == 2)
    {   
        int flag = 1;
        for(i = 0; i< first_num; i++)
        {
            for(j = 0; j < second_num; j++)
            {
                if( data[first[i]][second[j]] < distance)
                {    
                    flag = 0;
                    break;
                }
                if(!flag)
                    break;
            }
        }
        return flag;
    }
    // 均值
    else if(link_type == 3)
    {    
        int sum = 0;
        double ave = 0;
        for(i = 0; i< first_num; i++)
        {
            for(j = 0; j < second_num; j++)
            {
                sum += data[first[i]][second[j]];
            }
        }
        ave = sum / (first_num * second_num);
        if(ave >= distance)
            return 1;
        else
            return 0;
    }
}

// 组合子类 
vector<vector<int>> combine(vector<vector<int>> &list, vector<int> &first, vector<int>&second, int second_index, int first_index)
{
    vector<vector<int>> res;
    vector<int> temp;
    int i = 0;
    for(i = 0; i< first.size(); i++)
    {
        temp.push_back(first[i]);
    }
    for(i = 0; i< second.size(); i++)
    {   
        temp.push_back(second[i]);
    }
    sort(temp.begin(),temp.begin()+temp.size());

    res.push_back(temp);
    list.erase (list.begin() + second_index);
    list.erase (list.begin() + first_index);
    for(i = 0; i < list.size(); i++)
    {
        res.push_back(list[i]);
    }
  //  sort(res.begin(),res.begin()+res.size());

    return res;

}