// Comprehensive test showcasing all features
int count;
int sum;
bool done;

count = 1;
sum = 0;
done = false;

while (count < 5)
{
    sum = sum + count;
    count = count + 1;
}

if (sum > 10)
{
    print sum;
}
else
{
    sum = 0;
}
