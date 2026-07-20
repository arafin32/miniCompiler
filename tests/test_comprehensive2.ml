int count;
int sum;

count = 1;
sum = 0;

while (count < 5)
{
    sum = sum + count;
    count = count + 1;
}

if (sum > 10)
{
    print(sum);
}
else
{
    sum = 0;
}
